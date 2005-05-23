<center><h2>Comment creer son niveau</h2></center>


<h3>Description</h3>
<ul>
<li>Un Niveau est composé d'un ou plusieur modules, ainsi que quelques parametres globaux</li>
<li>Un Module est un object 3D possedant un mesh de collision, ainsi  que quelques parametres locaux.</li>
</ul>
<p>
La facon la plus simple de faire un nouveau Niveau et de réutiliser des modules deja existant, et de les placer ou vous le désirez.
</p>
<hr>
<br/>
<br/>

<center><h2>Niveau</h2></center>

<p>Un Niveau est un fichier texte dont l'extention est : <code>.level</code>, éditable avec n'importe quel editeur de texte (comme notepad)<br/>
Ce type de fichier est composé de bloc, chacun des blocs est délimité par <b>[</b> et <b>]</b><br/>

<p>exemple : </p>
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

Il y a 4 sortes de blocs :
<ul>
<li><b>Module</b> : ajoute un module au level.</li>
<li><b>StartPoint</b> : ajoute a point de depart, ou le joueur apparaitra en debut de session.</li>
<li><b>Camera</b> : specifie la position de la camera.</li>
<li><b>General</b>: ajoute des parametres globaux comme la gravité...</li>
</ul>
</p>
<br/>
<br/>

<h3>Module</h3>

<p>Un bloc doit contenir :</p>

<ul>
<li><b>pos</b> : x y z position du module en 3d par rapport aux axes X Y Z.</li>
<li><b>rot</b> : x y z w rotation du module en utilisant un axis angle (x y z = axe , w = angle de rotation autour de l axe)</li>
<li><b>scale</b> : x y z scale du module en 3d par rapport aux axes X Y Z.</li>
<li><b>name</b> nom du module (le nom de fichier correspondant est : "nom_du_module<b>.module</b>").</li>
</ul>

<p>voici un exemple, qui montre comment ajouter le module <code>snow_score50</code> au level, a la position (x=0,y=0,z=0) sans rotation et sans scale
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
<li><b>pos</b> : x y z position du point de depart sur les axes X Y Z.</li>
</lu>

<p>Vous devriez ajouter autant de StartPoint qu'il y a de joueur possible.</p>

<p>Cet exempel ajoute un StartPoint, qui fera partir un joueur de la position (0.09 1.5 9.6) :</p>

<table><tr><td>
<pre>
[StartPoint]
pos = 0.09 1.5 9.6
</pre>
</td></tr></table>

<h3>Camera</h3>

<ul>
<li><b>pos</b> : x y z position de la camera (au depart de la session) sur les axes X Y Z.</li>
</ul>

<p>
Si vous ne specifier qu'une seule camera, alors, les cameras de tous les joueurs, utlisirons ce réglage.
Sinon, la premiere camera specifié servira au joueur 1 , la deuxieme au joueur 2 , etc...<br/>
L'example suivant specifie une camera à la position (0.09 1.5 9.6) :</p>

<table><tr><td>
<pre>
[Camera]
pos = 0.09 1.5 9.6
</pre>
</td></tr></table>

Pour resumer, le joueur, son startpoint et sa camera sont liés ensemble.<br/>
Si vous avez un niveau ou 16 joueurs peuvent participer en meme temps,
vous devez spécifier 16 startpoints (un par joueur) et 16 cameras (chacune proche d'un startpoint)

<h3>General</h3>

<?php
    $level_array = array (
        array ("name"=>"levelName", "description"=>"Nom du Niveau (affiché avant le compte-a-rebourd)"),
        array ("name"=>"author", "description"=>"Nom de l'auteur"),
        array ("name"=>"maxOpenClose", "description"=>"Combien de fois au maximum le joueur peut appuyer sur la touche <b>Control</b> ."),
        array ("name"=>"timeBeforeStart", "description"=>"Temps avant de lancer le joeur."),
        array ("name"=>"timeBeforeRestart", "description"=>"Temps entre la fin d'une session et le debut d'une nouvelle."),
        array ("name"=>"timeBeforeCheck", "description"=>"Temps avant que le serveur ne verifie que la session est finie (la session est finie quand plus personne ne bouge)"),
        array ("name"=>"timeTimeout", "description"=>"Durrée maximal d'une session"),
        array ("name"=>"nbWaitingPlayers", "description"=>"Nombre minimal de joueur pour debuter une session"),
        array ("name"=>"openAccelCoef", "description"=>"Acceleration en mode vole"),
        array ("name"=>"closeAccelCoef", "description"=>"Acceleration en mode roule"),
        array ("name"=>"sphereDensity", "description"=>"Densitée du pingouin"),
        array ("name"=>"gravity", "description"=>"Specifie la gravité"),
        array ("name"=>"bounceWater", "description"=>"Energie emise quand un joueur touche l'eau (0=pas de rebond)"),
        array ("name"=>"bounceVelWater", "description"=>"Vitesse necessaire pour qu'un joueur qui touche l'eau fasse un rebond"),
        array ("name"=>"bounceClient", "description"=>"Energie emise quand un joueur touche un autre joueur"),
        array ("name"=>"bounceVelClient", "description"=>"Vitesse necessaire pour qu'un joueur qui touche un autre joueur fasse un rebond"),
        array ("name"=>"bounceScene", "description"=>"Energie emise quand un joueur touche le decor"),
        array ("name"=>"bounceVelScene", "description"=>"Vitesse necessaire pour qu'un joueur qui touche le decor fasse un rebond"),
        array ("name"=>"minVelBeforeEnd", "description"=>"Vitesse minimale pour qu'un joueur soit considéré comme a l'arret"),
        array ("name"=>"nbMaxPlayers", "description"=>"nombre maximum de joueur"),
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

<p>Chacune de ces valeurs est optionel.</p>
<hr>
<br/>
<br/>

<center><h2>Module</h2></center>
<p>
Un module est un élement grahique en 3D, combiné à un mesh de collision, chaque mocule possede ses propres parametres.
Les parametres sont : <b>score, acceleration, friction</b>.
La maniere simple de faire un nouveau module consiste à partir d'un module deja existant en gardant les graphisme mais en changeant ses parametres.
La maniere la plus dur , mais aussi la plus interessante et de creer un nouveau module sous 3ds Max(tm), et de l'exporter avec NeL
</p>
<p>
Un module est un fichier texte dont l'extension est <code>.module</code>, il est editable avec n'importe quel editeur de texte (vi,notepad.exe par ex)
Comme pour les fichier de Niveau, chaque bloc est délimité par <b>[</b> and <b>]</b><br/>
</p>

<p>exemple : </p>
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
Il y a 2 type de blocs :
<ul>
<li><b>TriColl</b> décrit un module de collision fait uniquement de triangle.</li>
<li><b>Box</b> décrit un module de collision fait uniquement de boites.</li>
</ul>

<?php
    $module_array = array (
        array ("name"=>"pos", "description"=>"x y z position du Module sur les axes X Y Z."),
        array ("name"=>"rot", "description"=>"x y z w rotation du Module en utilisant un axis angle (x y z = axe , w = angle)"),
        array ("name"=>"scale", "description"=>"x y z scale du Module sur les axes X Y Z"),
        array ("name"=>"score", "description"=>"score du joueur si il atterit sur ce module"),
        array ("name"=>"bounce", "description"=>"coeficient de rebond d'un joueur sur ce module en mode roule"),
        array ("name"=>"accel", "description"=>"coeficient d'acceleration si on presse haut sur ce module en mode roule"),
        array ("name"=>"friction", "description"=>"coeficient de friction d'un joueur sur ce module en mode roule"),
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

<center><h2>Glossaire</h2></center>

<h3>Repere</h3>
<p>
Le moteur du jeu utilise un repere main droite (comme dans 3ds Max(tm))
Le plan de l'eau est Z=1, un objet dont Z=10 est au dessus de l'eau.
</p>

<h3>Axis Angle</h3>
<p>
Un Axis Angle est un axe autour duquel on effectue une rotation
Par exemple cet Axis Angle : <code>0 0 1 1.57</code> fait une rotation de 90 degrée (Pi/2 ~ 1.57) autour de l'axe des Z.
</p>

<h3>Plus d'info</h3>
Pour avoir plus d'info sur les math, aller voir : <a href="http://mathforum.org/dr.math/"> cette page(en anglais).</a>

