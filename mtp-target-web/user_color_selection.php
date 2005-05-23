 <form action="" method="get" name="triqui">
	<div align="center">
		<table border="0" cellspacing="1" cellpadding="2" bgcolor ="#000000">
			<?php
			$counter = 0;
			$table = "";
				for ($x=1;$x<=6;$x++){
					$varx = dechex(51*($x-1));
					if ($varx == "0"){$varx = "00";}
					for ($y=1;$y<=6;$y++){
						$vary = dechex(51*($y-1));
						if ($vary == "0"){$vary = "00";}
						for ($z=1;$z<=6;$z++){
							$varz = dechex(51*($z-1));
							if ($varz == "0"){$varz = "00";}
							$color = "#".$varx.$vary.$varz;
							$counter++;
							if ($counter == 1){$table.= "<tr height = \"15\">";}
							$table .= "<td bgcolor = \"$color\" width = \"10\"\" style = \"cursor: hand\" onclick = \"triqui.p_color.value='$color'\"></td>";
							if($counter == 18){$counter = 0;$table.= "</tr>";}
						}
					}
				}
				echo $table; // don't forget this one :)
			?>
		</table>
		<br/>
		<input type="hidden" name="page" value="user_settings.php"><br/>
		You are picking color: <input type="text" name="p_color" size="7"><br/>
		<input type="submit" value="Select">
	</div>
</form>