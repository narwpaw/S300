//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Language.c                               *//
//*  Opis                         : S³ownik wyrazów                          *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 06.11.2007                               *//
//*  Data ostatniej modyfikacji   : --.--.----                               *//
//*--------------------------------------------------------------------------*//
//*                                                                          *//
//*--------------------------------------------------------------------------*//
//* Rejestr zmian  (data - opis):                                            *//
//*                                                                          *//
//*                               Brak zmian                                 *//
//*                                                                          *//
//*__________________________________________________________________________*// 
//****************************************************************************//
//****************************************************************************//

#include "Language.h"
                  
   
                  
const LangTab Speech[2]={
           //polski     Speech[g_leng].//    //
                    {  
                     "MENU",          //Menu
                     "Zm analog.",    //ZmAnalog
                     "Zm dwust.",   //ZmDwust
                     "Stany IO",      //StanIO
                     "Ustawienia",    //Ustawienia
                     "Rejestrator",   //Rejestr
                     "Info",          //Info
                     "Jezyk",         //Jêzyk
                     "Czas",          //Czas
                     "Czas g/m/s",    //CzasGMS
                     "Data",          //Data
                     "Data d/m/r",    //DataDMR
                     "Data/Czas",     //DataCzas
                     "Hasla",         //Hasla
                     "Polski",        //Polski
                     "Angielski",     //Angielski
                     "Nazwa prog.",   //NazwaProg
                     "Data modyfi.",  //DataMod
                     "Czas cyklu",    //CzasCyklu
                     "Zmiana hasla",  //ZmHasla
                     "Poziom 1",       //Poziom1
                     "Poziom 2",       //Poziom2
                     "Poziom 3",       //Poziom3
                     "Wej/Wyj",        //WeWy
                     "Poziom dostep",   //PoziomDost
                     "Obecne haslo:", //ObecneHaslo
                     "Nowe haslo:",   //NoweHaslo
                     "Wpisz",         //Wpisz
                     "Bledne",        //Bledne
                     "haslo",         //haslo
                     "Nowe",          //Nowe
                     "Haslo:",        //Haslo
                     "Restart ster.", //RestartSter
                     "Potwierdz",     //Potwierdz
                     "Sygnaly",       //Sygnaly;
                     "Monitoring",    //Obserwowane;
                     "Opcje",         //Opcje
                     "Odforsuj",      //Odforsuj
                     "Forsuj",         //Forsuj
                     "Monitoruj",      //Monitoruj
                     "Dostep",         //Dostep
                     "zastrzezony",    //zastrzezony
                     "Brak",           //Brak
                     "miejsca",        //miejsca
                     "Istnieje",       //Istnieje
                     "Puste",          //Puste
                     "Start ster.",    //UruchomienieSter
                     "Czysc rejestr",  //CzyscRej
                     "Czyszcz rej",    //CzyszczonyRej
                     "Konfig.",   //Konfiguracja
                     "Procedura",      //Procedura  
                     "Stan wej. ",   //Stan wejscia
                     "Neg. wej.",   //NegWejscia
                     "Stan wyj. ",   //Stan wyjêcia
                     "St. alert ",   //Stan zegara
                     "St. timer ",   //Stan timera
                     "St. tim. N ",   //Stan timera
                     "Ustaw wy",   //UstawWyjscie
                     "Wy.na 1",    //WlaczWyjscie[15];
                     "Wy. na 0",    // ZerujWyjscie[15];
                     "Timer rst. ",   //ResetTimer
                     "Zegar rst. ",   //ResetAlert
                     "Wl.Scene ",   //W³¹cz scenê
                     "Inc.Scen ",   //W³¹cz scenê
                     "Dec.Scen ",   //W³¹cz scenê
                     "Timer ",      //Timer
                     "Zegar ",      //Zegar
                     "Il. sekund: ",   //IlSek
                     "St. Sceny ",   //IlSek
                     "Dodaj wiersz   ",   //DodajWiersz
                     "Usun wiersz    ",   //UsuñWiersz
                     "Wiersz nr:     ",   //WierszNr
                     "z procedurami", //Zprocedurami
                     "Zapisz ", //Zapisz
                     "zmiany ", //Zmiany
                     "Styl 1", //Styl1
                     "Styl 2", //Styl2
                     "Styl 3", //Styl3
                     "Styl 4", //Styl3
                     "Styl 5", //Styl3
                     "Styl 6", //Styl3
                     "Styl 7", //Styl3
                     "Styl 8", //Styl3
                     "Ekran ",    //Ekran
                     "Grafika", //Grafika
                     "Numer scneny   ", //NumerSceny
                     "Zmiana ",
                     "Dotyk ",
                     "Dotknij ",  //Dotknij
                     "precyzyjnie ", //precyzyjnie
                     "wskaznik ", //wskaznik
                     "Haslo wl. ", //HasloWl
                     "Dzien tyg. ", //DzienTygodnia
                     "Rok ",
                     "Miesiac ",
                     "Dzien ",
                     "Godzina ",
                     "Minuta ",
                     "Sekunda ",
                     "Typ: ",
                     "Wartosc ",//char Wartosc[15];
                     "Przycisk ", 
                     "Akcja ",
                     "Otworz strone ",//OtworzStrone
                     "Wyl.", //Wyl
                     "Wl.", //Wl
                     "Wezly ",//Wêz³y
                     "Kanal RF ",  //RfChannel
                     "Kanal zajety ", //KanalZajety
                     "Wygaszacz",  //ScreenSever
                     "Godz. wl.", //TimeOn
                     "Godz. wyl.", //TimeOff
                     "Tryb Slave ",//TrybSlave
                     "Tryb Master ",//TrybMaster 
                     "Niedostepne ", //Niedostepne
                     "w trybie Slave ", //W_Trybie_Slave
                     "Dzwiek ",
                     "Thermostat ", //Termostat
                     "PWSCPSN ", //PWSCPSN
                     "Czujnik", //Czujnik[15];
                     "Wykonawca", //Wykonawca[15];
                     "Text",
                     "Ukryj" ,//Ukryj
                     "Ustaw" ,//Ustaw
                     "Termo.", //Termo_
                     "Temperatura", //Temperatura
                     "Terminarz", //Terminarz
                     "Serwis", //Zaawansowane[15];
                     "Czysc pamiec",//Czysc_pamiec
                     "Wyjdz", //Wyjdz 
                     "Wl.ochrone",//WlOchrone
                     "Zmien Haslo",//ZmienHaslo
                     "Syrena ", //Syrena
                     "Aktywacja za",//AktywacjaZa[15];
                     "Haslo poprawne",//HasloPoprawne
                     "Bledne haslo", //BledneHaslo
                     "Ochrona wyl.", //OchronaWyl
                     "Glosnik", //Glosnik  
                     "Wyladowana", //Wyladowana 
                     "bateria", //bateria 
                     "Sprawdz", //Sprawdz 
                     "liste wezlow", //liste_wezlow 
                     "Zmien ID", //ZmID
                     "Usun ID", //UsunID
                     "Zmien opis", //ZmOpis
                     "Stan We/Wy", //StanWeWy
                     "Test trans.", //TestTrans
                     "Dane wyslane",  //Dane_wyslane[15];
                     "Dane odebrane",  //Dane_odebrane[15];
                     "Dane utracone",// Dane_utracone[15];  
                     "Siec",         //Siec[15];
                     "Ustaw Wi-Fi", //UstawWiFi[15];
                     "Punkt dostepu", //Punkt_dostepu[15];
                     "Nazwa",    //Nazwa[15];
                     "Polacz",   //Polacz[15];
                     "Zmien",    //Zmien[15];
                     "Klucz seciowy", //Klucz_seciowy
                     "Domena",//Domena[15]
                     "Subdomena", //Subdomena
                     "Scezka (opcja)",//Scezka[15]
                     "Np. ceuron.pl",//char np_ceuron_pl[15];
                     "Np. /home",//char np_home[15];  
                     "Okresowosc", //Okresowosc
                     "Wl. WiFi",//Wl_WiFi
                     "Wyl. WiFi",//Wyl_WiFi
                     "Adres IP", //char Adres_IP[15;
                     "Maska", //char Maska[15];
                     "Brama", //char Brama[15];
                     "Serwer", //char Serwer[15]; 
                     "Polaczony",//char Polaczony[15];
                     "Niepolaczony",//char Niepolaczony[15];
                     "Transfer", //char Transfer[15];
                     "Status", //char Status[15];
                     "Jasnosc", // Jasnosc[15];
                     "Tryb aktywny",//char Tryb_aktywny[15];
                     "Tryb nieaktyw.",//char Tryb_nieaktywny[15];
                     "Rej. analog.",//char Analogi[15];
                     "Czekaj...",  //Czekaj
                     "Zmienne"//Razem
                    },
                    
                    
                    //Angielski
                    {  
                     "MENU",          //Menu
                     "Var analog.",   //ZmAnalog
                     "Var binar.",    //ZmDwust
                     "IO states",     //StanIO
                     "Setup",         //Ustawienia
                     "Register",      //Rejestr
                     "Info  ",        //Info
                     "Language",      //jêzyk
                     "Time",          //czas
                     "Time h/m/s",    //CzasGMS
                     "Date",          //Data
                     "Date d/m/y",    //DataDMR
                     "Date/Time",     //DataCzas
                     "Passwords",     //hasla
                     "Polish  ",      //Polski
                     "English  ",     //Angielski
                     "Appli. name",   //NazwaProg
                     "Modifi. Date",  //DataMod
                     "Cycle time",    //CzasCyklu
                     "Passwords",  //ZmHasla
                     "Level 1",       //Poziom1
                     "Level 2",       //Poziom2
                     "Level 3",       //Poziom3
                     "In/Out",        //WeWy
                     "Access level",   //PoziomDost
                     "Old password:",  //ObecneHaslo
                     "New password:",  //NoweHaslo
                     "Insert",         //wpisz
                     "Wrong",          //Bledne
                     "password",       //haslo
                     "New",            //Nowe
                     "Password:",      //Haslo
                     "Restart",        //RestartSter
                     "Confirm",        //Potwierdz
                     "Sygnals",        //Sygnaly;
                     "Monitoring",     //Obserwowane;
                     "Options",        //Opcje
                     "Unforce all",    //Odforsuj
                     "Force",          //Forsuj
                     "Monitoring",     //Monitoruj
                     "Access",         //Dostep
                     "denied",         //zastrzezony
                     "No",             //Brak
                     "space",          //miejsca
                     "Exist",          //Istnieje
                     "Empty",          //Puste
                     "Start cont.",   //UruchomienieSter
                     "Clean regist.",  //CzyscRej
                     "Clean regist",   //CzyszczonyRej
                     "Konfig.",   //Konfiguracja
                     "Procedure",       //Procedura  
                     "In.state ",   //StanWejscia
                     "Neg. In.",   //NegWejscia
                     "Out.state ",  //StanWyjœcia
                     "Clock st. ",   //Stan zegara
                     "Tiemer st. ",   //Stan timera
                     "Tim. st. N. ",   //Stan timera
                     "Set out ",   //UstawWyjscie
                     "Set Q to 1",    //WlaczWyjscie[15];
                     "Set Q to 0",    // ZerujWyjscie[15];
                     "Timer rst. ",   //ResetTimer
                     "Clock rst. ",   //ResetZegar
                     "Scene on ",   //Ustaw scene
                     "Inc.Scen. ",   //W³¹cz scenê
                     "Dec.Scen. ",   //W³¹cz scenê
                     "Timer ",      //Timer
                     "Alert ",      //Alert
                     "Seconds: ",   //IlSek
                     "Scene st. ",   //IlSek
                     "Add row ",   //DodajWiersz
                     "Remove row     ",   //UsuñWiersz
                     "Row nr:        ",   //WierszNr
                     "with proced.", //Zprocedurami
                     "Save",   //Zapisz
                     "changes", //Zmiany
                     "Design 1", //Styl1
                     "Design 2", //Styl2
                     "Design 3", //Styl3
                     "Design 4", //Styl3
                     "Design 5", //Styl3
                     "Design 6", //Styl3
                     "Design 7", //Styl3
                     "Design 8", //Styl3
                     "Screen ",    //Ekran
                     "Graphics ", //Grafika
                     "Scene number ", //NumerSceny
                     "Change ",
                     "Touch ",
                     "Touch ",  //Dotknij
                     "precisely ", //precyzyjnie
                     "pointer ", //wskaznik
                     "Pass. on. ", //HasloWl
                     "Day of week ", //DzienTygodnia
                     "Year ",
                     "Mounth ",
                     "Day ",
                     "Hour ",
                     "Minute ",
                     "Second ",
                     "Type: ",
                     "Value ",//char Wartosc[15];
                     "Button ",
                     "Action ",
                     "Open page ",//OtworzStrone
                     "Off", //Wyl
                     "Nodes ", //Wezly
                     "RF channel",  //RfChannel
                     "Channel Engage ", //ChannelEngage
                     "Screensever",  //ScreenSever
                     "Time on", //TimeOn
                     "Time off", //TimeOff
                     "Slave mode",//TrybSlave
                     "Master mode",//TrybMaster  
                     "Unavailable ", //Niedostepne
                     "in Slave mode ", //W_Trybie_Slave
                     "Sound ",
                     "Thermostat ", //Termostat
                     "MTWTFSS ", //PWSCPSN 
                     "Sensor", //Czujnik[15];
                     "Executor", //Wykonawca[15];
                     "Text",
                     "Hide" ,//Ukryj
                     "Setup" ,//Ustaw
                     "Thermo.", //Termo_
                     "Temperature", //Temperatura
                     "Timetable", //Terminarz
                     "Service", //Zaawansowane[15];
                     "Clean memory",//Czysc_pamiec
                     "Exit", //Wyjdz 
                     "Protection On",//WlOchrone
                     "Change pass.",//ZmienHaslo
                     "Siren", //Syrena alarmowa
                     "Activation for",//AktywacjaZa[15];
                     "Password OK",//WpiszHaslo
                     "Wrong password", //BledneHaslo
                     "Protection off", //OchronaWyl 
                     "Speaker", //Glosnik  
                     "Discharged", //Wyladowana 
                     "battery", //bateria 
                     "Check", //Sprawdz 
                     "node list", //liste_wezlow 
                     "Change ID", //ZmID
                     "Remove ID", //UsunID
                     "Change label", //ZmOpis
                     "I/O state",//StanWeWy
                     "Trans. Test", //TestTrans
                     "Data sent",  //Dane_wyslane[15];
                     "Data recived",  //Dane_odebrane[15];
                     "Data lost",// Dane_utracone[15];
                     "Network",         //Siec[15];
                     "Wi-Fi config.", //UstawWiFi[15];
                     "Acces point", //KreatorWiFi[15];
                     "Name", //Nazwa[15];
                     "Connect",   //Polacz[15];
                     "Change",    //Zmien[15];
                     "Network key", //Klucz_seciowy
                     "Domain",//Domena[15]
                     "Subdomain",//subdomena[15]
                     "Path(optional)",//Scezka[15]
                     "eg: ceuron.pl",//char np_ceuron_pl[15];
                     "eg: /home",//char np_home[15];  
                     "Periodicity", //Okresowosc
                     "WiFi On",//Wl_WiFi
                     "WiFi Off",//Wyl_WiFi 
                     "IP Adress", //char Adres_IP[15;
                     "Mask", //char Maska[15];
                     "Gateway", //char Brama[15];
                     "Server", //char Serwer[15]; 
                     "Connect",//char Polaczony[15];
                     "Disconnect",//char Niepolaczony[15];
                     "Transfer", //char Transfer[15];
                     "Status", //char Status[15];
                     "Brightness", // Jasnosc[15];
                     "Active Mode",//char Tryb_aktywny[15];
                     "Inactive Mode",//char Tryb_nieaktywny[15];
                     "Analogs reg.",//char Analogi[15];
                     "Wait...",  //czekaj
                     "Variables", //Razem
                    }
                      
                                    
                  }; 
