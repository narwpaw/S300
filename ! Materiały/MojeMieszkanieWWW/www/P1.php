<?
$MyPassword='';
function stripslashes_deep($value)
{
$value = is_array($value) ?
array_map('stripslashes_deep', $value) :
stripslashes($value);
return $value;
}
$has=stripslashes_deep($_POST['has']);
if (($has==$MyPassword) || ($MyPassword=='')) $pass='ok';
?>
<html>
<head>
<meta http-equiv="Content-type" content="text/html; charset=iso-8859-2" />
<title>CEURON Smart home</title>
<meta name="description" content="Ceuron smart home - user site" />
<meta name="keywords" content="inteligentny dom, automatyka budynku, automatyka, smart home,"/>
<meta name="classification" content="global,all" />
<meta name="robots" content="all,index,follow" />
<meta name="revisit-after" content="2 days"/>
<meta name="author" content="Ceuron">
<meta name="description" content="" />
<link rel="Shortcut icon" href="images/ikona.ico" />
<link rel="stylesheet" href="style.css" type="text/css" />

<script type="text/javascript" src="prototype.js"></script>
<script type="text/javascript" src="jquery.js"></script>

<?
if ($pass=='ok') echo'
<script>
var Vold = new Array();
var str;
var idx=0;
function PeriodFunction(){

        idx++;
       $.ajax({
	       cache: false,
	       url : "di3100137.csv",
	       success : function (data) {
			var arr = data.split('."'".'\n'."'".');
			str=arr;
		}
	});

        var Res='."'".'Recive:'."'".'+str;
        var V;
        var V2;
        var el1;
        var PZM=-1;
        var el = document.getElementById('."'".'Box0'."'".');
        el.innerHTML = '."'".'SERVER CONNECTIONS: '."'".'+idx;

}

var pe = new PeriodicalExecuter(PeriodFunction, 1);

function Clik(Zm,War){
 $.ajax({
   type: "POST",
   url: "putgate.php",
   data: "NodeID=3100137&Order=2&Content="+Zm+"="+War,
   success: function(msg){
   }
 });

}
function postwith (to,p) { 
  var myForm = document.createElement("form");
  myForm.method="post" ; 
  myForm.action = to ;
  for (var k in p) {
    var myInput = document.createElement("input") ;
    myInput.setAttribute("name", k) ;
    myInput.setAttribute("value", p[k]); 
    myForm.appendChild(myInput) ;
  }
  document.body.appendChild(myForm) ;
  myForm.submit() ; 
  document.body.removeChild(myForm) ;
  }
</script>
';
?>


</head>

<body>

<?
if ($pass=='ok'){ echo'
<div class="external">

  <div id="logo"><a href="www.ceuron.pl"></a></div>

  <div id="MainBox">

  <div id="Wallpaper">

   </div>

  <div id="Box0"></div>

  </div>
</div>
';
  }else{
   echo '<div class="external"><div id="PassBox">
        Insert password<br><br>
        <form action="" method="post">
	      <input type="password" name="has" /><br>
	      <input type="submit" value="OK" />
        </form>
   </div></div>';
  }
?>
</body>

<script>
<?
if ($pass=='ok') echo'
PeriodFunction();
';
?>
</script>
</html>
