function do_onload()
{
	toggleElementByName("expandable");
}

if (window.addEventListener)
window.addEventListener("load", do_onload, false)
else if (window.attachEvent)
window.attachEvent("onload", do_onload)
else if (document.getElementById)
window.onload=do_onload


function toggleElementById(cid) {
 if( document.getElementById(cid).style.display=='none' ){
   document.getElementById(cid).style.display = '';
 }else{
   document.getElementById(cid).style.display = 'none';
 }
}

function toggleElement(celement) {
//alert("toggle "+celement+" "+celement.className+" "+celement.id+" "+celement.style.display)

 if( celement.style.display=='none' ){
   celement.style.display = '';
 }else{
   celement.style.display = 'none';
 }

}

function toggleElementByName(cname)
{
//	ccollect=new Array()
	var inc=0
	//var alltags=document.all? document.all : document.getElementsByTagName("*")
	var alltags=document.getElementsByTagName("TR")
	for (i=0; i<alltags.length; i++)
	{
		if(alltags[i].className==cname)
			toggleElement(alltags[i]);
	}
}