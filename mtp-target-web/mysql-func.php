<?php


function validInput($input)
{
    return (ereg("^[A-Za-z0-9]+$",$input));
}

function validPage($page)
{
    return (ereg("^[/\.A-Za-z0-9-]+$",$page));
}

?>
