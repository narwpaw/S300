object MainForm: TMainForm
  Left = 154
  Top = 325
  Width = 1440
  Height = 725
  Caption = 'WizFi'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 125
  TextHeight = 16
  object Label1: TLabel
    Left = 696
    Top = 40
    Width = 33
    Height = 16
    Caption = 'COM:'
  end
  object Label2: TLabel
    Left = 856
    Top = 40
    Width = 12
    Height = 16
    Caption = 'IP'
  end
  object Memo1: TMemo
    Left = 8
    Top = 16
    Width = 681
    Height = 633
    Lines.Strings = (
      '')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object Wyslij: TButton
    Left = 696
    Top = 112
    Width = 137
    Height = 41
    Caption = 'Wyslij'
    TabOrder = 1
    OnClick = WyslijClick
  end
  object ComNr: TEdit
    Left = 736
    Top = 32
    Width = 33
    Height = 24
    TabOrder = 2
    Text = '1'
  end
  object Polacz: TButton
    Left = 696
    Top = 64
    Width = 137
    Height = 41
    Caption = 'Po'#322#261'cz z ruterem'
    TabOrder = 3
    OnClick = PolaczClick
  end
  object Memo2: TMemo
    Left = 696
    Top = 304
    Width = 689
    Height = 337
    Lines.Strings = (
      'POST /test/test.php HTTP/1.1'
      'User-Agent: Fiddler'
      'Host: www.ceuron.pl'
      'Content-Length: 66'
      'Content-Type: application/x-www-form-urlencoded'
      ''
      'NodeID=1234567&Key=987654321&Order=1&Content='
      'v01=1'
      'v02=1'
      'v03=0')
    TabOrder = 4
  end
  object IP: TComboBox
    Left = 880
    Top = 32
    Width = 145
    Height = 24
    ItemHeight = 16
    ItemIndex = 1
    TabOrder = 5
    Text = 'www.ceuron.pl'
    Items.Strings = (
      'www.wp.pl'
      'www.ceuron.pl')
  end
end
