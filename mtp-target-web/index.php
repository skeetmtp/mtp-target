<?php
ob_start ("ob_gzhandler");
include_once("mysql-func.php");
bench_start();
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
<div id="footer">
<?php
	include_once("footer.php");
	$page_generation_duration = bench_end();
	printf("page generated in %s secondes",$page_generation_duration);
?>
</div>
</body>
</html>
