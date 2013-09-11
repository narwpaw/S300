//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Language.h                               *//
//*  Opis                         : Biblioteka s³ownika wyrazów              *//
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

#ifndef _Language_
#define _Language_


#define Komentarz0_PL   "Ustaw okresowosc polaczen z serwerem"
#define Komentarz0_En   "Set the periodicity of server connections"

#define Komentarz1_PL   "Przyklad:\n   a.bbb.pl/cc\n\n Domena: bbb.pl\nSubdomena: a\n Sciezka: /cc"
#define Komentarz1_En   "Przyklad:\n   a.bbb.pl/cc\n\n Domena: bbb.pl\nSubdomena: a\n Path: /cc"

#define Komentarz2_PL   "Zly adres IP"
#define Komentarz2_En   "Wrong IP adress"

#define Komentarz3_PL   "Zly adres maski "
#define Komentarz3_En   "Wrong mask adress"

#define Komentarz4_PL   "Zly adres bramy "
#define Komentarz4_En   "Wrong gateway adress"

typedef struct _LangTab
                {
                  char MENU[15];
                  char ZmAnalog[15];  
                  char ZmDwust[15];
                  char StanIO[15];
                  char Ustawienia[15];
                  char Rejestr[15];
                  char Info[15];
                  char Jezyk[15];
                  char Czas[15];
                  char CzasGMS[15];
                  char Data[15];
                  char DataDMR[15];
                  char DataCzas[15];
                  char Hasla[15];
                  char Polski[15];
                  char Angielski[15];
                  char NazwaProg[15];
                  char DataMod[15];
                  char CzasCyklu[15];
                  char ZmHasla[15];
                  char Poziom1[15];
                  char Poziom2[15];
                  char Poziom3[15];
                  char WeWy[15];
                  char PoziomDost[15];
                  char ObecneHaslo[15];
                  char NoweHaslo[15];
                  char Wpisz[15];
                  char Bledne[15];
                  char haslo[15];
                  char Nowe[15];
                  char Haslo[15];
                  char RestartSter[15];
                  char Potwierdz[15];
                  char Sygnaly[15];
                  char Obserwowane[15];
                  char Opcje[15];
                  char Odforsuj[15];
                  char Forsuj[15];
                  char Monitoruj[15];
                  char Dostep[15];
                  char zastrzezony[15];
                  char Brak[15];
                  char miejsca[15];
                  char Istnieje[15];
                  char Puste[15];
                  char UruchomienieSter[15];
                  char CzyscRej[15];
                  char CzyszczonyRej[15];
                  char Konfiguracja[15];
                  char Procedura[15];
                  char StanWejscia [15];
                  char NegWejscia [15];
                  char StanWyjscia[15];
                  char StanZegara[15];
                  char StanTimera[15];
                  char StanTimeraNeg[15];
                  char UstawWyjscie[15];
                  char WlaczWyjscie[15];
                  char ZerujWyjscie[15];
                  char ResetTimer[15];
                  char ResetZegar[15];
                  char W³Scene[15];
                  char IncScene[15];
                  char DecScene[15];
                  char Timer[15];
                  char Zegar[15];
                  char IleSek[15];
                  char StSceny[15];
                  char DodajWiersz[15];
                  char UsunWiersz[15];
                  char WierszNr[15];
                  char Zprocedurami[15];
                  char Zapisz[15];
                  char Zmiany[15];
                  char Styl1[15];
                  char Styl2[15];
                  char Styl3[15];
                  char Styl4[15];
                  char Styl5[15];
                  char Styl6[15];
                  char Styl7[15];
                  char Styl8[15];
                  char Ekran[15];
                  char Grafika[15];
                  char NumerSceny[15];
                  char Zmiana[15];
                  char Dotyk[15];
                  char Dotknij[15];
                  char precyzyjnie[15];
                  char wskaznik[15];
                  char HasloWl[15];
                  char DzienTygodnia[15];
                  char Rok[15];
                  char Miesiac[15];
                  char Dzien[15];
                  char Godzina[15];
                  char Minuta[15];
                  char Sekunda[15];
                  char Typ_[15];
                  char Wartosc[15];
                  char Przycisk[15];
                  char Akcja[15];
                  char OtworzStrone[15];
                  char Wyl[15];
                  char Wl[15];
                  char Wezly[15];
                  char RfChannel[15];
                  char KanalZajety[15];
                  char ScreenSever[15];
                  char TimeOn[15];
                  char TimeOff[15];
                  char TrybSlave[15];
                  char TrybMaster[15];
                  char Niedostepne[15];
                  char W_Trybie_Slave[15];
                  char Dzwiek[15];
                  char Termostat[15];
                  char PWSCPSN[15]; 
                  char Czujnik[15];
                  char Wykonawca[15];
                  char Text[15];
                  char Ukryj[15];
                  char Ustaw[15];
                  char Termo_[15];
                  char Temperatura[15];
                  char Terminarz[15];
                  char Serwis[15];
                  char Czysc_pamiec[15];
                  char Wyjdz[15];
                  char WlOchrone[15];
                  char ZmienHaslo[15];
                  char Syrena[15];
                  char AktywacjaZa[15];
                  char HasloPoprawne[15];
                  char BledneHaslo[15];
                  char OchronaWyl[15];
                  char Glosnik[15];
                  char Wyladowana[15];
                  char bateria[15];
                  char Sprawdz[15]; 
                  char liste_wezlow[15];
                  char ZmID[15];
                  char UsunID[15];
                  char ZmOpis[15];
                  char StanWeWy[15];
                  char TestTrans[15];
                  char Dane_wyslane[15];
                  char Dane_odebrane[15];
                  char Dane_utracone[15];
                  char Siec[15];
                  char UstawWiFi[15];
                  char Punkt_dostepu[15];
                  char Nazwa[15];
                  char Polacz[15];
                  char Zmien[15];
                  char Klucz_seciowy[15];
                  char Domena[15];
                  char Subdomena[15];
                  char Scezka[15];
                  char np_ceuron_pl[15];
                  char np_home[15];
                  char Okresowosc[15];
                  char Wl_WiFi[15];
                  char Wyl_WiFi[15];
                  char Adres_IP[15];
                  char Maska[15];
                  char Brama[15];
                  char Serwer[15]; 
                  char Polaczony[15];
                  char Niepolaczony[15];
                  char Transfer[15];
                  char Status[15];
                  char Jasnosc[15];
                  char Tryb_aktywny[15];
                  char Tryb_nieaktywny[15];
                  char Analogi[15];
                  char Czekaj[15];
                  char Zmienne[15];
                }LangTab;

#endif
