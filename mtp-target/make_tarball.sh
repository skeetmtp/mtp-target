#!/bin/sh -

DEST=tmp/mtp-target/client
rm -rf tmp
mkdir tmp
mkdir tmp/mtp-target
mkdir $DEST
cp -a client/src/client $DEST
strip $DEST/client
cp -a client/data $DEST
cp -a client/cache $DEST
cp -a client/mtp_target_default.cfg $DEST
strip $HOME/install/release/lib/*.so
cp -a $HOME/install/release/lib/libnel3d.so.0.0.4 $DEST
cp -a $HOME/install/release/lib/libnel3d.so.0 $DEST
cp -a $HOME/install/release/lib/libnelnet.so.0.0.4 $DEST
cp -a $HOME/install/release/lib/libnelnet.so.0 $DEST
cp -a $HOME/install/release/lib/libnelmisc.so.0.0.4 $DEST
cp -a $HOME/install/release/lib/libnelmisc.so.0 $DEST
cp -a $HOME/install/release/lib/libnel_drv_opengl.so.0.0.0 $DEST
cp -a $HOME/install/release/lib/libnel_drv_opengl.so.0 $DEST
echo "#!/bin/sh -" >>$DEST/launch.sh
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:." >>$DEST/launch.sh
echo "./client" >>$DEST/launch.sh
chmod 700 $DEST/launch.sh
cd tmp
tar zcvf ../mtp-target-1.0.2.tgz mtp-target
