unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TMainForm = class(TForm)
    Memo1: TMemo;
    Wyslij: TButton;
    ComNr: TEdit;
    Label1: TLabel;
    Polacz: TButton;
    Memo2: TMemo;
    Label2: TLabel;
    IP: TComboBox;
    procedure WyslijClick(Sender: TObject);
    procedure PolaczClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;
  ComFile      : THandle;



const
  dcb_fBinary = $0001;
  dcb_fParity = $0002;
  dcb_fOutxCtsFlow = $0004;
  dcb_fOutxDsrFlow = $0008;

  // -- fDtrControl --
  DTR_CONTROL_ENABLE = $0010;
  DTR_CONTROL_HANDSHAKE = $0020;

  dcb_fDsrSensitivity = $0040;
  dcb_fTXContinueOnXoff = $0080;
  dcb_fOutX = $0100;
  dcb_fInX  = $0200;
  dcb_fErrorChar = $0400;
  dcb_fNull = $0800;

  // -- fRtsControl --
  RTS_CONTROL_ENABLE = $1000;
  RTS_CONTROL_HANDSHAKE = $2000;
  RTS_CONTROL_TOGGLE = $3000;

  dcb_fAbortOnError = $4000;

  cbInQueue = 256;
  cbOutQueue = 256;
//______________________________________________________________________________



implementation

{$R *.dfm}



function OpenCOMPort(Port :string): Boolean;
var
  DeviceName: array[0..80] of Char;

begin
   { First step is to open the communications device for read/write.
     This is achieved using the Win32 'CreateFile' function.
     If it fails, the function returns false.


   }
  StrPCopy(DeviceName, Port);

  ComFile := CreateFile(DeviceName,
    GENERIC_READ or GENERIC_WRITE,
    0,
    nil,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0);

  if ComFile = INVALID_HANDLE_VALUE then
    Result := False
  else
    Result := True;
end;


function SetupCOMPort: Boolean;
const
  RxBufferSize = 256;
  TxBufferSize = 256;
var
  DCB: TDCB;
  Config: string;
  CommTimeouts: TCommTimeouts;

begin


  Result := True;

  if not SetupComm(ComFile, RxBufferSize, TxBufferSize) then
    Result := False;

  if not GetCommState(ComFile, DCB) then
    Result := False;

  // define the baudrate, parity,...
  Config := 'baud=115200 parity=n data=8 stop=1';

  if not BuildCommDCB(@Config[1], DCB) then
    Result := False;

  if not SetCommState(ComFile, DCB) then
    Result := False;

  with CommTimeouts do
  begin
    ReadIntervalTimeout         := 1;
    ReadTotalTimeoutMultiplier  := 1;
    ReadTotalTimeoutConstant    := 1000;  //1000
    WriteTotalTimeoutMultiplier := 1;
    WriteTotalTimeoutConstant   := 1000;   //1000
  end;

  if not SetCommTimeouts(ComFile, CommTimeouts) then
    Result := False;
end;


{
  The following is an example of using the 'WriteFile' function
  to write data to the serial port.

  Folgendes Beispiel verwendet die 'WriteFile' Funktion, um Daten
  auf den seriellen Port zu schreiben.
}


procedure SendText(s: string);
var
  BytesWritten: DWORD;
begin
   {
     Add a word-wrap (#13 + #10) to the string

     An den übergebenen String einen Zeilenumbruch (#13 + #10) hängen
   }
  s := s + #13 + #10;

  //for (i:=1 to Length(s)) do BuffTx[i]=s[i];
  WriteFile(ComFile, s[1], Length(s), BytesWritten, nil);
end;


procedure SendBufor(s: array of byte; ByteToSend :integer );
var
  BytesWritten: DWORD;

begin
   {
     Add a word-wrap (#13 + #10) to the string

     An den übergebenen String einen Zeilenumbruch (#13 + #10) hängen
   }
  //s := s + #13 + #10;
  WriteFile(ComFile, s[0], ByteToSend, BytesWritten, nil);
end;




function ReadText(MaxResByte : byte): string;
var
  d: array[1..255] of char;
  s: string;
  BytesRead, i: DWORD;
  ReadOverlapped:TOverlapped;
begin
  Result := '';
  if not ReadFile(ComFile, d, MaxResByte, BytesRead, nil) then
  begin
    { Raise an exception }
  end;
  s := '';
  for i := 1 to BytesRead do s := s +d[I];
  Result := s;
end;


procedure CloseCOMPort;
begin
  CloseHandle(ComFile);
end;



procedure TMainForm.PolaczClick(Sender: TObject);
var

   BuffRx  : string;
   LastBuffRx  : string;
   Odp: boolean;
   ERROR : boolean;
   i : integer;
begin
Wyslij.Enabled:=false;
Polacz.Enabled:=false;
Memo1.Text:='';
OpenCOMPort('COM'+ComNr.Text);
SetupCOMPort;

SendText('ATC0');                             BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
SendText('AT+WD');                                  BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
SendText('AT+NDHCP=1');                             BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;

//SendText('AT+NSET=192.168.14.214,255.255.240.0,192.168.0.21');                                  BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
//SendText('AT+WWPA=iasemlocal');                     BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;

//ustawienie hasla
SendText('AT+WWEP1=0158000000');                     BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;

SendText('AT+WWPA=0158000000');                     BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;


//listing dostepnych sieci
SendText('AT+WS');                                    BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;







//SendText('AT+DNSSET=192.168.0.9,156.17.108.1');                             BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;

//SendText('AT+WA=IASE_local_wireless');         BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
//SendText('AT+WAUTO=0,default');         BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;


//polaczenie z ruterem
 SendText('AT+WA=default');         BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(250);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;


//SendText('AT+WA');                                    BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
//        BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;




SendText('ATC1');         BuffRx:=ReadText(160);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
   //SendText('AT+DNSLOOKUP=www.ceuron.pl');                             BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;




//SendText('ATA');                                    BuffRx:=ReadText(40);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
    i:=1;
    BuffRx:='';
    Odp:=false;
    while (i<15) and not Odp  do
    begin
        LastBuffRx:=BuffRx;
        BuffRx:=ReadText(250);
        if (BuffRx<>'') then Memo1.Text:=Memo1.Text+trim(BuffRx);
        if (LastBuffRx<>'') and (BuffRx='')  then Odp:=true;
        inc(i);
    end;
    refresh;

    sleep(20000);
    Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;




    CloseCOMPort;
Wyslij.Enabled:=true;
Polacz.Enabled:=true;
end;



procedure TMainForm.WyslijClick(Sender: TObject);
var

   BuffRx  : string;
   LastBuffRx  : string;
   Odp: boolean;
   ERROR : boolean;
   i : integer;
   Siz : integer;
   Buff : Array[1..5000] of byte;
   IPstr : string;
begin
Wyslij.Enabled:=false;
Memo1.Text:='';
OpenCOMPort('COM'+ComNr.Text);
SetupCOMPort;


 BuffRx:='';

SendText('AT+DNSLOOKUP=ceuron.pl');                             BuffRx:=ReadText(60);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;



   if (IP.ItemIndex=0) then begin SendText('AT+NCTCP=212.77.100.101,80');  end;
   if (IP.ItemIndex=1) then begin SendText('AT+NCTCP=31.170.162.26,80');         end;

   BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;



if (BuffRx<>'') then
begin


 sleep(2000);

 Buff[1]:=$1B;
 Buff[2]:=$53;
 Buff[3]:=$30;
SendBufor(Buff,3);
//SendText('S0');
SendText(Memo2.Text);
//SendText('E');
 Buff[1]:=$1B;
 Buff[2]:=$45;

SendBufor(Buff,2);
BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
    i:=1;
    BuffRx:='';
    Odp:=false;
    while (i<20) and not Odp  do
    begin
        LastBuffRx:=BuffRx;
        BuffRx:=ReadText(250);
        if (BuffRx<>'') then Memo1.Text:=Memo1.Text+trim(BuffRx);
        if (LastBuffRx<>'') and (BuffRx='')  then Odp:=true;
        inc(i);
    end;
    refresh;



SendText('AT+NCLOSEALL');                   BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;

 BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;
BuffRx:=ReadText(255);   Memo1.Text:=Memo1.Text+trim(BuffRx)+char($0D)+char($0A);  refresh;





end;


CloseCOMPort;
Wyslij.Enabled:=true;
end;



end.
