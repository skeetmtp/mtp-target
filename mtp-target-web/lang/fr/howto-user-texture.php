
<center><h2>Comment changer l'apparence de mon avatar.</h2></center>

<h3>Changer la texture de l'avatar</h3>
<ul>
<li>Recuperez <a href="http://mtptarget.free.fr/download">object viewer</a>.</li>
<li>Lancez le, et suivez les instruction d'installation</li>
<li>Avec votre logiciel d'edition d'image preferé, modifiez <code>object-viewer/data/ping_ball_blue.tga</code> </li>
<li>Verifiez l'aspect dans object viewer<br>
Pour ouvrir un mesh :<br>
Menu File/open selectionez votre mesh (<code>entity_pingoo_close.shape</code>)<br>
<code>Object Viewer</code> demande un fichier *.skel faites <i>Annuler</i><br>
Zommez en arriere avec la roulette de la souris (ou avec la touche shift + le bouton gauche)<br>
Pour tourner autour du perso : Touche Alt + click gauche<br>
Pour centrer le perso : Touche Control + click gauche<br>
</li>
<li>Une fois satisfait, renommez <code>ping_ball_blue.tga</code> en quelque chose comme : <code>ping_ball_votredescription.tga</code></li>
<li>N'oubliez pas : Sauvez au format <b>TGA 32 bit sans RLE</b>. Le nom du fichier doit suivre la regle : commencer par <code>ping_ball_</code> suivi de la descritipn de votre texture en fini par <code>.tga</code><br>
Par exemple, <code>ping_ball_ryzom.tga</code></li>
<li>Envoyez la sur notre serveur <a href="<?php echo "$user_texture" ?>">via cette page</a></li>
<li>Selectionez la <a href="?page=user_texture_selection.php">ici</a></li>
</ul>

<h3>Changer la couleur de l'avatar</h3>
<ul>

<li>Ouvrez le fichier <code>mtp_target.cfg</code> contenu dans le repertoire du jeu (par defaut <code>C:\Program Files\mtp-target\client</code>)</li>
<li>Ajoutez la ligne suivante : <br>
<code>EntityColor = { 255, 255, 255 };</code><br>
mais avec vos propres valeur { Rouge, Vert, Bleu }

</ul>



