<center><h2>Comment creer un serveur Windows(c)</h2></center>

<h3>Server Internet</h3>

<p>
Vous devez savoir que les port TCP 51574 et 80 doivent etre ouvert.<br>
Ouvrez le fichier nomé <code>mtp_target_service_default.cfg</code> avec votre editeur de texte favoris,
le chemin par defaut pour ce fichier est : <code>C:\Program Files\mtp-target</code>.
</p>

Regler ces variables comme ci-dessous :
<ul>

<li> declarer le serveur au gestionaire de serveur
    <pre>DontUseLS = 1;</pre>
</li>

<li> specifier l'IP du gestionaire de serveur
    <pre>LSHost = "195.68.21.201";</pre>
</li>

<li> specifier le nom de votre serveur
    <pre>ListenAddr = "thename.ofyourserver.org";</pre>
</li>

<li> specifier votre propre <code>ShardId</code> (Si vous en avez pas, <a href="<?php echo $contact_email;?>">mailez-nous</a> ou essayer un numero au hazard)
    <pre>ShardId = 123;</pre>
</li>

</ul>

<p>
Maintenant, il ne vous reste plus qu'a lancer <code>Server.exe</code>.
Si il'y a une erreure, essayez de changer de ShardId, et verifiez que vous n'avez pas fait d'erreures dans le fichier de config.
Le server tourne, donc vous pouvez tester en lancant le client du jeu, en mode Internet.
Votre serveur devrait être dans la liste des serveur disponible.
Maintenant vous pouvez <a href="<?php echo $contact_email;?>">nous envoyer un emaill</a>, pour que l'on puisse lui attribuer un nom :)
</p>


<h3>Serveur en reseau local</h3>

<p>Les parametres par defaut sont reglés pour lancer le serveur en mode reseau local,
donc, lancer simplement <code>Server.exe</code>. Si il y a un probleme, vérifiez dans le fichier <code>mtp_target_service_default.cfg</code> la valeur :
<pre>DontUseLS = 0;</pre>
</p>

<p>
Les joueurs doivent lancer le programme de setup afin de regler le Reseau en mode LAN, et y spécifier l'addresse IP du serveur.
</p>

