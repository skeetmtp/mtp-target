
<center><h2>How to customize my avatar.</h2></center>

<h3>Changing avatar texture</h3>
<ul>
<li>Get the <a href="http://mtptarget.free.fr/download">object viewer program</a>.</li>
<li>Run it and follow install instruction</li>
<li>With your prefered drawing tool modify <code>object-viewer/data/ping_ball_blue.tga</code> </li>
<li>Check visual aspect with object viewer<br/>
To open your mesh :<br/>
Menu File/open select your mesh (<code>entity_pingoo_close.shape</code>)<br/>
<code>Object Viewer</code> asks for a *.skel file : press <i>Cancel</i><br/>
mouse wheel down to zoom out (or shift & mouseleft button)<br/>
To rotate avatar : Alt key + left mouse button<br/>
To center avatar : Control key + left mouse button<br/>
</li>
<li>When your are satisfied, rename <code>ping_ball_blue.tga</code> to something like : <code>ping_ball_yourdescription.tga</code></li>
<li>Remember : Save it in <b> TGA format without RLE </b>. The file name *must* follow the name rules: it must starts with <code>ping_ball_</code> and, after, the name of your texture and <code>.tga</code><br/>
For example, a valid name is <code>ping_ball_ryzom.tga</code></li>
<li>Upload it to our server <a href="?page=user_upload_texture">using this page</a></li>
<li>Select it <a href="?page=user_texture_selection">here</a></li>
</ul>

<h3>Changing avatar color</h3>
<ul>

Add the following line to <code>mtp_target.cfg</code> :<br/> 
<code>EntityColor = { 255, 255, 255 };</code><br/>
with your own color value { Red, Green, Blue }

</ul>



