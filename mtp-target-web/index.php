<?php
include_once("menu.php");

if ($page=="todo.txt") echo "<pre>";
if($page=="news.txt")
  include ("newsparser.php");
else
  include($page);

if ($page=="todo.txt") echo "</pre>";

?>

</div>
</td></tr>
</table>
</body>
</html>
