<center><h2>Comment mettre en place son server.</h2></center>

<h3>Serveur sur Internet</h3>

<p>
D'abord il faut savoir que les ports 51574 et 80 doivent etre ouvert.
Ouvrez le fichier nomé <code>mtp_target_service.cfg</code> avec un editeur de texte.
</p>

Ajouter les lignes suivantes :
<ul>

<li> Doit se trouver au tout debut du cfg
    <pre>RootConfigFilename   = "mtp_target_service_default.cfg";<pre>
</li>

<li> mettre l'addresse de votre serveur
    <pre>ListenAddr = "thename.ofyourserver.org";</pre>
</li>

<li> mettez ici votre propre <code>ShardId</code> (Si vous en avez pas, <a href="<?php echo $contact_email;?>">mailez nous</a> ou essayez une valeur au hasard)
    <pre>ShardId = 321;</pre>
</li>

</ul>

<p>
Maintenant, il ne vous reste plus qu'a lancer le serveur.
Si il y a une erreure, essayez de changer de ShardId, et vérifiez que vous n'avez pas fait d'erreure dans le fichier de configuration.
Maintenant que le serveur est lancé, vous pouvez essayer le mode internet avec le client.
Votre serveur devrait apparaitre dans la liste des serveur disponible.
Si vous disposez d'une IP statique ou d'un systeme de nommage d'IP dynamique (comme dyndns,dtdns...), vous pouvez <a href="<?php echo $contact_email;?>">nous envoyer un mail</a>, pour que nous puissions associer un nom a votre IP
</p>


<h3>serveur en reseau local</h3>

<p>Les réglages par defaut permettent de lancer directement un serveur en mode local, donc vous avez juste a lancer l'executable.
</p>

<p>
Vous pouvez maintenant lancer le client en utilisant le mode LAN.
</p>

<h3>Les different réglages possibles</h3>

<p>C'est une liste des parametres interessant(a completer)</p>

<ul>
<li>Activer les level de type 1(easy) et 2(expert) :
    <pre>ReleaseLevel         = { 1, 2 };</pre>
</li>

<li>Desactiver les optimisation reseaux(plus de bande passante consommée mais moins de saccades) déjà à 1 par defaut :
    <pre>DisableNetworkOptimization = 1;</pre>
</li>
</ul>

