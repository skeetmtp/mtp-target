<center><h2>How to set up a server</h2></center>

<h3>Internet server</h3>

<p>
First you should know that TCP port 51574 and 80 must be open.<br>
Open the file named <code>mtp_target_service.cfg</code> with your favorite text editor.
</p>

Add the following variables :
<ul>

<li> Must be the first line in the cfg
    <pre>RootConfigFilename   = "mtp_target_service_default.cfg";<pre>
</li>

<li> set the name of your server
    <pre>ListenAddr = "thename.ofyourserver.org";</pre>
</li>

<li> set your own <code>ShardId</code> (If you don't have one, <a href="<?php echo $contact_email;?>">email us</a> or try a random one)
    <pre>ShardId = 321;</pre>
</li>

</ul>

<p>
Now, all you have to do is launch the server.
If there is an error, try to change the ShardId, and check you don't make a mistake in the config file.
Since the server is running you can try with the game client, using internet mode.
Your server should be in the list of available server.
If you have a static IP or DNS hosting service (like dyndns,dtdns...), you can  <a href="<?php echo $contact_email;?>">send us an email</a>, so we can set a name associated to your ip
</p>


<h3>LAN server</h3>

<p>The default settings should let you launch a LAN Server, so just launch the server.
</p>

<p>
You can run the game client, using LAN mode.
</p>

<h3>Server settings</h3>

<p>Here is a list of usefull settings (not complete)</p>

<ul>
<li>Enable level of type 1(easy) et type 2(expert) :
    <pre>ReleaseLevel         = { 1, 2 };</pre>
</li>

<li>Disable network optimization(more bandwidth needed but less jerks) default = 1:
    <pre>DisableNetworkOptimization = 1;</pre>
</li>
</ul>

