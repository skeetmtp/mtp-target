<center><h2>How to create a level</h2></center>


<h3>Description</h3>
<ul>
<li>A level is made of one or more module and some global parameters</li>
<li>A module is a 3d object with a collision shape and some local parameters.</li>
</ul>
<p>
The most simple way to make new level is tu reuse existing modules, and to place theme where you want.
</p>
<hr>
<br/>
<br/>

<center><h2>Level</h2></center>

<p>A level is a text file with the <code>.level</code> extension, it is editable with any text editor like notepad.<br/>
This kind of file is made of blocs, each bloc is delimited by <b>[</b> and <b>]</b><br/>

<p>example : </p>
<table><tr><td>
<pre>
[General]
LevelName = Where are you
Author = Skeet and Ace

[Module]
Pos = 0.0 0.0 1.0
Name = snow_target_50_flat

[StartPoint]
Pos = 0.3 21.4 9.2

[Camera]
Pos = 0 0 0
</pre>
</td></tr></table>

There is 4 kind of blocs :
<ul>
<li><b>Module</b> : add a module to the level.</li>
<li><b>StartPoint</b> : add a player start point, from where the player will spawn at the begining of the session.</li>
<li><b>Camera</b> : set the camera position.</li>
<li><b>General</b>: add global parameter like gravity...</li>
</ul>
</p>
<br/>
<br/>

<h3>Module</h3>

<p>A bloc module must contains :</p>

<ul>
<li><b>pos</b> : x y z position of the module in 3d over X Y Z axes.</li>
<li><b>rot</b> : x y z w rotation of the module using axis angle (x y z = axis , w = angle)</li>
<li><b>scale</b> : x y z scale of the module in 3d over X Y Z axes.</li>
<li><b>name</b> module name (corresponding filename is "modulename<b>.module</b>").</li>
</ul>

<p>Here an example, to add the module <code>snow_score50</code> to the level, at position (x=0,y=0,z=0) without any rotation and unit size
</p>

<table><tr><td>
<pre>
[Module]
pos = 0 0 0
rot = 0 1 0 0
scale = 1 1 1
name = snow_score50
</pre>
</td></tr></table>
<br/>
<br/>

<h3>StartPoint</h3>

<lu>
<li><b>pos</b> : x y z position of the StartPoint in 3d over X Y Z axes.</li>
</lu>

<p>You should add as many StartPoint as maximum player number.</p>

<p>The following example, set a StartPoint so one player will start at position (0.09 1.5 9.6) :</p>

<table><tr><td>
<pre>
[StartPoint]
pos = 0.09 1.5 9.6
</pre>
</td></tr></table>

<h3>Camera</h3>

<ul>
<li><b>pos</b> : x y z position of the Starting Camera in 3d over X Y Z axes.</li>
</ul>

<p>
If you set only one Camera, all starting camera of all players will be this one,
Otherwise the first camera will be used for the first player and so on...
The following example, set a Camera to position (0.09 1.5 9.6) :</p>

<table><tr><td>
<pre>
[Camera]
pos = 0.09 1.5 9.6
</pre>
</td></tr></table>

So Player, StartPoint and Camera a really dependent from each others.
For example, in 16 players level, you should add 16 StartPoint (one for each player) and
16 Camera (one near each StartPoint)

<h3>General</h3>

<?php
    $level_array = array (
        array ("name"=>"levelName", "description"=>"Name of the level (displayed before the final countdown)"),
        array ("name"=>"author", "description"=>"Name of the author (displayed before the final countdown)"),
        array ("name"=>"maxOpenClose", "description"=>"How many time the player can press the <b>Control</b> key."),
        array ("name"=>"timeBeforeStart", "description"=>"Time before launching the players."),
        array ("name"=>"timeBeforeRestart", "description"=>"Time between end of a session a start of a new one."),
        array ("name"=>"timeBeforeCheck", "description"=>"Time before the server check if the session is finished(a session is finished when nobody is moving)"),
        array ("name"=>"timeTimeout", "description"=>"Max duration of a session"),
        array ("name"=>"nbWaitingPlayers", "description"=>"Minimum number of players needed to start a session"),
        array ("name"=>"openAccelCoef", "description"=>"Flying mode acceleration value"),
        array ("name"=>"closeAccelCoef", "description"=>"Rolling mode acceleration value"),
        array ("name"=>"sphereDensity", "description"=>"Density of the pingoo"),
        array ("name"=>"gravity", "description"=>"Set the global gravity"),
        array ("name"=>"bounceWater", "description"=>"Energy emited when a player touch the water(0=no bounce)"),
        array ("name"=>"bounceVelWater", "description"=>"Minimum velocity to do a bounce when a player touche the water"),
        array ("name"=>"bounceClient", "description"=>"Energy emited when a player touch another player"),
        array ("name"=>"bounceVelClient", "description"=>"Minimum velocity to do a bounce when a player touche another player"),
        array ("name"=>"bounceScene", "description"=>"Energy emited when a player touch the decor"),
        array ("name"=>"bounceVelScene", "description"=>"Minimum velocity to do a bounce when a player touch the decor"),
        array ("name"=>"minVelBeforeEnd", "description"=>"Minimum velocity before the player is considered as stoped"),
        array ("name"=>"nbMaxPlayers", "description"=>"Maximum player number"),
        );
?>

<table border="0" CELLPADDING=4 CELLSPACING=2>
<tr>
    <th bgcolor="#ffffff"><center>&nbsp;Name&nbsp;</center></th>
    <th bgcolor="#ffffff"><center>&nbsp;Description&nbsp;</center></th>
</tr>
<?php
                $count = 0;
                foreach($level_array as $key => $value)
                {
                      if(($count%2)==0)
                        $color =  "#cccccc";
                      else
                        $color =  "#dddddd";
                      printf("<td bgcolor=%s>",$color);
                      printf("%s",$value["name"]);
                      printf("</td><td bgcolor=%s>",$color);
                      printf("%s",$value["description"]);
                      printf("</td></tr>");
                      $count++;
                }
?>
</table>

<p>Each of this values a optional.</p>
<hr>
<br/>
<br/>

<center><h2>Module</h2></center>
<p>
A module is a 3D graphic element combined with a collision mesh, each module could have their own parameters values.
Available parameters are : <b>score, acceleration, friction</b>.
The easy way to make a new module is to start from an existing module keeping the visual art and only change parameters.
The hard way but the most interesting is to make a new designed module in 3ds Max(tm), and export it with NeL.
</p>
<p>
A module is a text file with the <code>.module</code> extension, it is editable with any text editor like notepad.<br/>
As Level file, each bloc is delimited by <b>[</b> and <b>]</b><br/>
</p>

<p>example : </p>
<table><tr><td>
<pre>
[TriColl]
nbVertex = 3
4.777 -15.3475 1.8286
5.77333 -15.4707 1.8286
6.11222 -14.8615 1.82858
nbFace = 1
0 1 2
score = 0
bounce = 1
accel = 0.0001
friction = 30

[Box]
pos = -0.104996 -17.3242 7.47563
rot = 1 0 0 0
scale = 70 70 5
score = 50
bounce = 1
accel = 0
friction = 100
</pre>
</td></tr></table>

<br/>
There is 2 kind of blocs :
<ul>
<li><b>TriColl</b> describe a triangle only collision module.</li>
<li><b>Box</b> describe a box only  collision module.</li>
</ul>

<?php
    $module_array = array (
        array ("name"=>"pos", "description"=>"x y z position of the Module in 3d over X Y Z axes"),
        array ("name"=>"rot", "description"=>"x y z w rotation of the module using axis angle (x y z = axis , w = angle)"),
        array ("name"=>"scale", "description"=>"x y z scale of the Module in 3d over X Y Z axes"),
        array ("name"=>"score", "description"=>"score the player get if he lands on this module"),
        array ("name"=>"bounce", "description"=>"bounce coeficient when the palyer touch the module in rolling mode"),
        array ("name"=>"accel", "description"=>"acceleration coeficient when a player press up on this module"),
        array ("name"=>"friction", "description"=>"firction coef, that is how the player slow down when he touch this module"),
        );
?>

<table border="0" CELLPADDING=4 CELLSPACING=2>
<tr>
    <th bgcolor="#ffffff"><center>&nbsp;Name&nbsp;</center></th>
    <th bgcolor="#ffffff"><center>&nbsp;Description&nbsp;</center></th>
</tr>
<?php
                $count = 0;
                foreach($module_array as $key => $value)
                {
                      if(($count%2)==0)
                        $color =  "#cccccc";
                      else
                        $color =  "#dddddd";
                      printf("<td bgcolor=%s>",$color);
                      printf("%s",$value["name"]);
                      printf("</td><td bgcolor=%s>",$color);
                      printf("%s",$value["description"]);
                      printf("</td></tr>");
                      $count++;
                }
?>
</table>

<hr>
<br/>
<br/>

<center><h2>Glossary</h2></center>

<h3>Coordinate Systeme</h3>
<p>
All the game engine, use a right handed coordinate system (As 3ds max does).
The Water is the Z=1 plan, an object with Z=10 is above the water.
</p>

<h3>Axis Angle</h3>
<p>
An Axis Angle consists of a vector designating the axis to rotate about, and the rotation amount to use.
For example this Axis Angle : <code>0 0 1 1.57</code> do a 90 degree(Pi/2 ~ 1.57) rotation around Z axe.
</p>

<h3>More info</h3>
For mor information about math check out :  <a href="http://mathforum.org/dr.math/">this page.</a>

