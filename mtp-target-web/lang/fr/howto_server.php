<h1>Comment créer un serveur</h1>

<p>Cette page explique comment mettre en place un serveur en local ou sur Internet.</p>

<h2>Serveur sur Internet</h2>

<p>Il faut tout d'abord éditer le fichier de configuration du serveur qui s'appelle <code>mtp_target_service_default.cfg</code> qui est dans le répertoire d'installation, par défaut <code>C:\Program Files\mtp-target</code>.</p>

<ul>

<li>Mettre la variable <code>DontUseLS</code> à 1.
    <pre>DontUseLS = 1;</pre>
</li>

<li>Mettre variable <code>LSHost</code> à "195.68.21.201".
    <pre>LSHost = "195.68.21.201";</pre>
</li>

<li>Mettre votre adresse IP dans la variable <code>ListenAddress</code> (si votre adresse change, vous devez la mettre à jour sinon les joueurs ne pourront pas se connecter chez vous), par exemple:
    <pre>ListenAddr = "acemtp.dyndns.org";</pre>
</li>

<li>Mettre la variable <code>ShardId</code> à une valeur qui n'existe pas déjà. Soit vous essayez un nombre au pif, soit vous me demandez un numéro par mail, par exemple:
    <pre>ShardId = 154;</pre>
</li>

</ul>

<p>Il ne reste plus qu'à lancer le programme <code>Server.exe</code>. Si une erreur se produit,
c'est surement que le numéro du ShardId est déjà pris ou alors que vous avez
fait une erreur dans le fichier de configuration. Une fois le serveur bien lancé, vous pouvez lancer le jeu
en mode Internet, votre serveur doit alors apparaitre dans la liste des serveurs. Ensuite vous pouvez m'envoier un mail pour me dire ce que vous voullez que je mette comme texte à coté de votre serveur.</p>

<p>Le serveur a besoin d'avoir accès au port 51574 en TCP et en UDP ainsi que le port 49999 en TCP.</p>

<h2>Serveur sur un réseau local</h2>

<p>Vous avez pas besoin de modifier le fichier de configuration, vous avez qu'a lancer
le programme <code>Server.exe</code>.</p>

<p>Par contre, les joueurs doivent lancer l'outil de configuration pour mettre l'adresse IP de votre serveur et lancer le jeu en mode LAN.</p>

<hr>
<p>Si vous avez des problèmes pour lancer un serveur, contactez moi <a href="<?php echo $contact_email;?>">ici</a></p>

<div id="footer"><hr><pre>Last updated: $Id$</pre></div>
