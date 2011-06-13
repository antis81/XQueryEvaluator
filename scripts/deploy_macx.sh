#!/bin/sh
# Copyright 2007 David Johnson
# The author grants unlimited permission to
# copy, distribute and modify this script

APPNAME=XQueryEvaluator
OUTPATH=build

### get system configuration ########################################

# as long as we can find qmake, we don't need QTDIR
FWPATH=`qmake -query QT_INSTALL_LIBS`
if [ ! -d $FWPATH/QtGui.framework ] ; then
    echo "ERROR: cannot find the Qt frameworks. Make sure Qt is installed"
    echo "and qmake is in your environment path."
    exit
fi

### get required user input #########################################

if [ -z $APPNAME ] ; then
    echo
    echo "This script prepares a Qt application bundle for deployment. It will"
    echo "copy over the required Qt frameworks and sets the installation"
    echo "identifications. Please see the \"Deploying an Application on Qt/Mac\""
    echo "page in the Qt documentation for more information."
    echo
    echo "This script assumes you have already built the application bundle."
    echo
    echo -n "What is the name of the application? "
    read userinput
    APPNAME=$userinput
fi

BUNDLE=$OUTPATH/$APPNAME.app
APPBIN="$BUNDLE/Contents/MacOS/$APPNAME"

if [ ! -d $BUNDLE ] ; then
    echo "ERROR: cannot find application bundle \"$BUNDLE\" in current directory"
    exit
fi

if [ ! -x $APPBIN ] ; then
    echo "ERROR: cannot find application in bundle. Did you forget to run make?"
    exit
fi

echo "application: $APPNAME"
echo "bundle:      $BUNDLE"

echo "Add Qt frameworks? (y/N): "
read userinput
if [ "$userinput" == "y" ] ; then

    ### query binary for frameworks #####################################

    echo -n "Using frameworks"
    for n in `otool -L $APPBIN | grep Qt` ; do
        path=`echo $n | grep Qt`
        if [ $path ] ; then
    	name=`basename $path`
    	FRAMEWORKS="$FRAMEWORKS $path"
    	echo -n " $name"
        fi
    done
    echo

    ### make install ####################################################

    #echo "Running make install"
    #if [ -e Makefile.Release ] ; then
    #    make -f Makefile.Release install
    #elif [ -e Makefile ] ; then
    #    make install
    #else
    #    echo "ERROR: Makefile not found. This script requires the macx-g++ makespec"
    #fi
    #strip $APPBIN

    ### copy over frameworks ############################################

    mkdir -p $BUNDLE/Contents/Frameworks
    for path in $FRAMEWORKS ; do
        name=`basename $path`
        if [ ! -d "$FWPATH/$name.framework" ] ; then
            echo "ERROR: cannot find $FWPATH/$name.framework"
            exit
        fi
        echo "Copying $name framework"
        cp -fR $FWPATH/$name.framework $BUNDLE/Contents/Frameworks
        # strip libs (-x is max allowable for shared libs)
        strip -x $BUNDLE/Contents/Frameworks/$name.framework/Versions/4/$name
    done

    # remove unwanted parts
    find $BUNDLE/Contents/Frameworks | egrep "debug|Headers" | xargs rm -rf

    ### set the identification names for frameworks #####################

    echo -n "Setting framework IDs..."

    echo -n "Setting framework IDs..."

    for path in $FRAMEWORKS ; do
        name=`basename $path`
        echo -n " $name"
        install_name_tool \
    	-id @executable_path/../Frameworks/$name.framework/Versions/4/$name \
            $BUNDLE/Contents/Frameworks/$name.framework/Versions/4/$name
    done
    echo

    ### change framework paths ##########################################

    echo -n "Changing framework paths for $APPNAME..."
    for path in $FRAMEWORKS ; do
        name=`basename $path`
        echo -n " $name"
        install_name_tool \
    	-change $path \
            @executable_path/../Frameworks/$name.framework/Versions/4/$name \
            $APPBIN
    done
    echo

    ### change location for bundled frameworks #########################

    echo -n "Fixing bundled frameworks..."
    for fwpath in $FRAMEWORKS ; do
        fwname=`basename $fwpath`
        echo -n " $fwname"
        framework="$BUNDLE/Contents/Frameworks/$fwname.framework/Versions/4/$fwname"
        # get framework dependencies
        deps=""
        for n in `otool -LX $framework | grep Qt` ; do
    	dep=`echo $n | grep Qt`
    	if [ $dep ] ; then
    	    deps="$deps $dep"
	    fi
        done
        # fix dependency path
        for path in $deps ; do
        	name=`basename $path`
        	if [ "$name" != "$fwname" ] ; then
    		    install_name_tool \
        		-change $path \
        		@executable_path/../Frameworks/$name.framework/Versions/4/$name \
        		$framework
    	    fi
        done
    done
    echo

    ### misc cleanup ###############################################

    find $BUNDLE/Contents | egrep "CVS" | xargs rm -rf

fi

### copy languages ###############################################
echo "Deploying translations ..."
lrelease $APPNAME.pro
cp -v translations/*.qm $BUNDLE/Resources/translations
echo "Removing generated translation binaries ..."
#rm -f translations/*.qm
echo

### create disk image ###############################################

echo "Creating disk image"
imagedir="/tmp/$APPNAME.$$"
mkdir $imagedir
cp -R $BUNDLE $imagedir

# TODO: copy over additional files, if any
hdiutil create -ov -srcfolder $imagedir -format UDBZ -volname "$APPNAME" "$APPNAME.dmg"
hdiutil internet-enable -yes "$APPNAME.dmg"
rm -rf $imagedir

echo "Done"