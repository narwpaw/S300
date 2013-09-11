<?
$ID='3100137';
$Exe='.csv';

function stripslashes_deep($value)
{
$value = is_array($value) ?
array_map('stripslashes_deep', $value) :
stripslashes($value);

return $value;
}


$NodeID=stripslashes_deep($_POST['NodeID']);
$Key=stripslashes_deep($_POST['Key']);
$Order=stripslashes_deep($_POST['Order']);
$Content=stripslashes_deep($_POST['Content']);




if ($NodeID=='')
{
echo'
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl" lang="pl">
<head>
  <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
  <title>CEURON</title>
<meta name="description" content="Inteligentny dom dla ka|dego" />
<meta name="keywords" content="inteligentny dom, automatyka budynku, automatyka"/>
<meta name="classification" content="global,all" />
<meta name="robots" content="all,index,follow" />
<meta name="revisit-after" content="2 days"/>
<meta name="author" content="Ceuron">
  <meta name="description" content="" />
  <link rel="Shortcut icon" href="images/ikona.ico" />
  <link rel="stylesheet" href="ceuron.css" type="text/css" />
</head>
<body>
';


   echo ('Wizualizacja strony <br />');
   
   $nazwa_pliku = "di".$ID.$Exe;

if (is_readable($nazwa_pliku))
  {
   if ($plik = fopen($nazwa_pliku, "r"))
     {
      $dane = fread($plik, filesize($nazwa_pliku));

      if ($dane === FALSE) echo "Odczyt danych z pliku nie powiódB si...";
        else echo "odczytane dane: ".$dane."<br />";

      fclose($plik);

     } else echo "Nie mog nawiza poBczenia z plikiem...";

  } else echo "Z pliku nie mo|na odczyta informacji lub on nie istnieje...";


   
echo'
</body>
</html>
   ';
   
}//-----------------------------------------------------------------------------
else if ($NodeID==$ID)
{

             if ($Order==1) //zapis do pliku DataIn wszystkich zmiennych .*
             {
                $dane = $Content;


                
                $file = "di".$ID.$Exe; // przypisanie zmniennej $file nazwy pliku
                $fp = fopen($file, "w+");     // uchwyt pliku, otwarcie do dopisania
                rewind($fp);                  // ustawienie kursora na pocztku pliku
                flock($fp, 2);                // blokada pliku do zapisu
                fwrite($fp, $dane);           // zapisanie danych do pliku
                flock($fp, 3);                // odblokowanie pliku
                fclose($fp);                  // zamknicie pliku

             }
             else if ($Order==2) //zapis do pliku  DataIn pojedyDczej zmiennej.*
             {
                $nazwa_pliku = "di".$ID.$Exe; // przypisanie zmniennej $file nazwy pliku
                $plik = fopen($nazwa_pliku, "r");
                $DaneTmp = fread($plik, filesize($nazwa_pliku));
                fclose($plik);

                $zmienna=substr($Content, 0, 3);
                $Poz1=strpos($DaneTmp, $zmienna);

                $TypZm=substr($zmienna, 0, 1);
                
                if (($TypZm=='b') || ($TypZm=='d'))
                {
                    $RozZm=1;
                }else if (($TypZm=='a') || ($TypZm=='c'))
                {
                    $RozZm=4;
                }else{
                    $warotsc=no;
                }
                $warotsc=substr($Content, 4, $RozZm);
                $PozPocz=0;
                $PozPocz=4+$Poz1;
                $Dane=substr($DaneTmp, 0, $PozPocz).$warotsc.substr($DaneTmp, $PozPocz+$RozZm);


                //$dane = $DaneTmp;
                echo('   Dane: '.$Dane.'   zmienna:'.$zmienna.'   Poz1: '.$Poz1.'   warotsc: '.$warotsc.'   RozZm: '.$RozZm.'   Dane: '.$Dane.'   Rozmiar: '.strlen($Dane));
                //echo('   Dane: '.$Dane);


                $file = "di".$ID.$Exe; // przypisanie zmniennej $file nazwy pliku
                $fp = fopen($file, "w+");     // uchwyt pliku, otwarcie do dopisania
                rewind($fp);                  // ustawienie kursora na pocztku pliku
                flock($fp, 2);                // blokada pliku do zapisu
                fwrite($fp, $Dane);           // zapisanie danych do pliku
                flock($fp, 3);                // odblokowanie pliku
                fclose($fp);                  // zamknicie pliku

             }
             //else if ($Order==3)  
            if (($Order==3)  || ($Order==2))//zapis do pliku  DataOut pojedyDczej zmiennej.*     //!!!!   ($Order==2) -tymczasowo 
             {
             $Index=1;
             $Index--;
             $Content=trim($Content);
             if (file_exists("do".$ID.$Exe))
             {
                $nazwa_pliku = "do".$ID.$Exe; // przypisanie zmniennej $file nazwy pliku
                $plik = fopen($nazwa_pliku, "r");
                $DaneOdczyt = fread($plik, filesize($nazwa_pliku));
                fclose($plik);
                
                //ograniczenie ilo[ci danych -----------------------------------
                $PZ=0;
                $zl=0;
                for ($idx=0; $idx<6; $idx++)
                {
                    $zl++;
                    $NP=strpos($DaneOdczyt, "\r\n", $PZ);
                    if ($NP===False)
                    {
                          $idx=7;
                    }else{
                          $PZ=$NP+2;
                    }
                    
                }
                if (($PZ>0) && ($zl>4))
                {
                   $PZ=$PZ-2;
                   $DaneOdczyt=substr($DaneOdczyt, 0, $PZ);
                }
                //--------------------------------------------------------------
                
                $IndexTxt=$DaneOdczyt[0].$DaneOdczyt[1].$DaneOdczyt[2].$DaneOdczyt[3];
                $Index=(int)$IndexTxt;
              }

                $Index++;
                $Content[3]=';';
                $Dane=trim($Index).';'.$Content."\r\n".$DaneOdczyt;
                
                echo ('  Index:'.$Index.' Dane:'.$Dane);
                $file = "do".$ID.$Exe; // przypisanie zmniennej $file nazwy pliku
                $fp = fopen($file, "w+");     // uchwyt pliku, otwarcie do dopisania
                //rewind($fp);                  // ustawienie kursora na pocztku pliku
                flock($fp, 2);                // blokada pliku do zapisu
                fwrite($fp, $Dane);           // zapisanie danych do pliku
                flock($fp, 3);                // odblokowanie pliku
                fclose($fp);                  // zamknicie pliku

             }
             
}else{
  echo("Error");

}



?>
 
