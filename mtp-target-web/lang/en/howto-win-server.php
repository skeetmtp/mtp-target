<center><h2>How to create a Windows(c) server</h2></center>

<h3>Internet server</h3>

<p>
First you should know that TCP port 51574 and 80 sould be open.<br>
Open the file named <code>mtp_target_service_default.cfg</code> with your favorite text editor,
the default path of this file is <code>C:\Program Files\mtp-target</code>.
</p>

Set the following variables :
<ul>

<li> declare server to the server manager
    <pre>DontUseLS = 1;</pre>
</li>

<li> set the server manager ip
    <pre>LSHost = "195.68.21.201";</pre>
</li>

<li> set the name of your server
    <pre>ListenAddr = "thename.ofyourserver.org";</pre>
</li>

<li> set your own <code>ShardId</code> (If you don't have one, <a href="<?php echo $contact_email;?>">email us</a> or try a random one)
    <pre>ShardId = 123;</pre>
</li>

</ul>

<p>
Now, all you have to do is launch <code>Server.exe</code>.
If there is an error, try to change the ShardId, and check you don't make a mistake in the config file.
Since the server is running you can try with the game client, using internet mode.
Your server should be in the list of available server.
Now you can  <a href="<?php echo $contact_email;?>">send us an email</a>, so we can set a name to your server :)
</p>


<h3>LAN server</h3>

<p>The default settings should let you launch a LAN Server,
so just launch <code>Server.exe</code>. If it does'nt work check in <code>mtp_target_service_default.cfg</code> file :
<pre>DontUseLS = 0;</pre>
</p>

<p>
All the player have to launch the setup program, to set the LAN Server name or IP.
</p>

