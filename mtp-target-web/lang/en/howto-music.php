<center><h2>How change music.</h2></center><br>
<br>
You used to like the Mtp Target music made by <a href="http://hulud.digital-murder.org/">Hulud</a>, but now (after playing some hours), you want to listen to something else ?<br>
I will explain how to create and use a m3u play list with winamp 3, if you don't use winamp try <a href="http://www.google.com/search?q=m3u+create">google m3u create</a> for your favorite mp3 player.<br>
First select you mp3 files and drop them into winamp. Your winamp play list window should be filled with your mp3s.<br>
Now press the "save list" button of play list window : <br>
<?php echo "<img src=\"$image_dir/img/winamp_m3u_a.jpg\">"; ?><br>
<br>
Go to Mtp Target <b>client</b> directory and save it to <code>test.m3u</code><br>
<?php echo "<img src=\"$image_dir/img/winamp_m3u_b.jpg\">"; ?><br>
Your m3u is saved.<br>
<br>
Now add this line in your mtp_target.cfg :<br>
<br>
<code>M3UList			= "test.m3u";</code><br>
<br>
You can launch and play Mtp Target, listening to your new music list.<br> 

<br>
<br> 
