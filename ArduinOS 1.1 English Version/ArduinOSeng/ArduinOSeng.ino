#include <PS2Keyboard.h>
#include <SD.h>
#include <TMRpcm.h>
#define CHAR_ESC '\x1B'

//PC SPEAKER MUST BE PLUG IN PIN 6(MEGA) AND 5V TO WORK

//PIN DATA - MODIFY VALUES IF YOU MUST
static int     keyboardCLK = 2;
static int     keyboardDAT = 3;
static int     SDPIN = 53;
static int     SDChipSelect = 8;
//END PIN DATA

PS2Keyboard    keyboard;
static String  keyboardCommand = "";
static String  keyboardLastKeyDown = "";
static boolean keyboardPressed=false;
static boolean keyboardAction;
static String  root = "/SYSTEM/";
static String  path = root;

void setup()
  {
  keyboard.begin(keyboardDAT, keyboardCLK);
  createRootFolder();
  Serial.begin(57600);
  delay(1000);
  clearScreen();
  clearScreen();
  Serial.println("");
  Serial.print("Starting ArduinOS 1.1...");
  Serial.println("");
  Serial.println("");
  Serial.print("A:\\>");
  }

void loop()
  {
  keyboardRead();
  if (keyboardLastKeyDown=="{ENTER}")
    {
    checkCommand(keyboardCommand);
    keyboardPressed=true;
    keyboardCommand="";
    keyboardLastKeyDown="";
    printBL();
    printLN("A:");
    printLN(getPath());
    printLN(">");
    }
  }
 
void printLN(String a)
  {
  Serial.print(a);
  }

void printBL()
  {
  Serial.println("");
  }

void clearScreen()
  {
  Serial.write(CHAR_ESC);
  Serial.write('E');
  }

void keyboardRead()
  {
  if (keyboard.available())
    {
    char c = keyboard.read();
    int ascii = c;
    keyboardPressed=false;
    
    if (keyboardPressed==false)
      {
      if ((ascii>=42 & ascii<=57) | (ascii>=97 & ascii<=122) |ascii==32)
         {
         keyboardPrint(String((char) ascii));
         }
      if (ascii==13)
        {
        keyboardLastKeyDown="{ENTER}";
        keyboardPressed=true;
        printBL();
        }
      if (ascii==27)
        {
        keyboardLastKeyDown="{ESCAPE}";
        keyboardPressed=true;
        }
      if (ascii==127)
        {
        keyboardLastKeyDown="{BACKSPACE}";
        keyboardPressed=true;
        }
      if (ascii==8)
        {
        keyboardLastKeyDown="{LEFT}";
        keyboardPressed=true;
        }
      if (ascii==21)
        {
        keyboardLastKeyDown="{RIGHT}";
        keyboardPressed=true;
        }
      if (ascii==10)
        {
        keyboardLastKeyDown="{DOWN}";
        keyboardPressed=true;
        }
      if (ascii==11)
        {
        keyboardLastKeyDown="{UP}";
        keyboardPressed=true;
        }
      }
    }  
  }
  
void keyboardPrint(String a)
  {
  printLN(a);
  keyboardCommand.concat(a);
  keyboardLastKeyDown=a;
  keyboardPressed=true;
  }

void checkCommand(String command)
  {
  keyboardAction = false;
  if (command=="dir"){checkCommandPreExecute();commandDir();}
  if (command=="ver"){checkCommandPreExecute();commandVer();}
  if (command=="mem"){checkCommandPreExecute();commandMem();}
  if (command=="cls"){checkCommandPreExecute();commandCls();}
  if (command=="reset"){checkCommandPreExecute();commandReset();}
  if (command=="cd" | command.substring(0,3)=="cd " | command=="cd.." |  command=="cd."){checkCommandPreExecute();commandCd(command);}
  if (command=="md" | command.substring(0,3)=="md "){checkCommandPreExecute();commandMd(command);}
  if (command=="rd" | command.substring(0,3)=="rd "){checkCommandPreExecute();commandRd(command);}
  if (command=="del" | command.substring(0,4)=="del "){checkCommandPreExecute();commandDel(command);}
  if (command=="ren" | command.substring(0,4)=="ren "){checkCommandPreExecute();commandRen(command);}
  if (command=="type" | command.substring(0,5)=="type "){checkCommandPreExecute();commandType(command);}
  if (command=="play" | command.substring(0,5)=="play "){checkCommandPreExecute();commandPlay(command);}
  if (command=="copy" | command.substring(0,5)=="copy "){checkCommandPreExecute();commandCopy(command);}
  if (command.substring(0,9)=="copy con " ){checkCommandPreExecute();commandCopyCon(command);}
  if (command!="" & keyboardAction==false)
    {
    checkCommandPreExecute();
    printBL();
    printLN("Bad command or file name.");
    printBL();
    }
  }

void checkCommandPreExecute()
  {
  keyboardAction=true;
  }

void commandRen(String a)
  {
  if (a=="ren" | a=="ren ")
    {
    printBL();
    printLN("You must input two filenames.");
    printBL();
    }
  if (a.substring(0,4)=="ren " & a.length()>4)
    {
    boolean error;
    error = false;
    if (count_spaces(a)>2 | count_spaces(a)<2)
      {
      error=true;
      }
    String first;
    first = a.substring(0,a.lastIndexOf(" "));
    if (first.length()<=4)
      {
      error = true;
      }
      else
      {
      first = first.substring(first.lastIndexOf(" ") + 1,first.length());
      }
    String second;
    second = a.substring(a.lastIndexOf(" ") + 1,a.length());
    if (second=="")
      {
      error = true;
      }
      else
      {
      second = second.substring(second.lastIndexOf(" ") + 1,second.length());
      if (second.substring(0,1)=="/")
        {
        second = root.substring(0,root.length()-1) + second;
        }
        else
        {
        second = path + second;
        }
      if (second.substring(second.length()-1,second.length())=="/")
        {
        second = second + first;
        }
        else
        {
        second = second + "/" + first;
        }
      }
    if (error==true)
      {
      printBL();
      printLN("ERROR. Expected: REN OLD NEW");
      printBL();
      }
      else
      {
      printBL();
      printLN("PENDING FUNCTION");
      printBL();
      }
    }
  }

void commandCopyCon(String a)
  {
  if (a=="copy con" | a=="copy con ")
    {
    printBL();
    printLN("You must input a file.");
    printBL();
    }
  }

void commandCopy(String a)
  {
  if (a=="copy" | a=="copy ")
    {
    printBL();
    printLN("Excepted: COPY FILE DESTINY");
    printBL();
    }
  boolean executedCopyCon = false;
  if (a.substring(0,9)=="copy con " & a.length()>9)
    {
    executedCopyCon=true;
    a = path + a.substring(9,a.length());
    if (checkIfFileExists(a)==false)
      {
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      keyboardCommand="";
      keyboardLastKeyDown="";
      File myFile;
      myFile = SD.open(charBuf, FILE_WRITE);
      if (myFile)
        {
        while (keyboardLastKeyDown!="{ESCAPE}")
          {
          keyboardRead();
          if (keyboardLastKeyDown!="" & keyboardLastKeyDown!="{ESCAPE}")
            {
            if (keyboardLastKeyDown=="{ENTER}")
              {
              myFile.println();
              keyboardLastKeyDown="";
              }
              else
              {
              myFile.print(keyboardLastKeyDown);
              keyboardLastKeyDown="";
              }
            }
          }
        }
      myFile.close();
      printBL();
      printLN("1 file copied.");
      printBL();
      keyboardPressed=true;
      keyboardCommand="";
      keyboardLastKeyDown="";
      }
      else
      {
      printBL();
      printLN("The file already exists.");
      printBL();
      }
    }
  if (executedCopyCon==false)
    {
    if (a.substring(0,5)=="copy " & a.length()>5)
      {
      boolean error;
      error = false;
      if (count_spaces(a)>2 | count_spaces(a)<2)
        {
        error=true;
        }
      String first;
      first = a.substring(0,a.lastIndexOf(" "));
      if (first.length()<=5)
        {
        error = true;
        }
        else
        {
        first = first.substring(first.lastIndexOf(" ") + 1,first.length());
        }
      String second;
      second = a.substring(a.lastIndexOf(" ") + 1,a.length());
      if (second=="")
        {
        error = true;
        }
        else
        {
        second = second.substring(second.lastIndexOf(" ") + 1,second.length());
        if (second.substring(0,1)=="/")
          {
          second = root.substring(0,root.length()-1) + second;
          }
          else
          {
          second = path + second;
          }
        if (second.substring(second.length()-1,second.length())=="/")
          {
          second = second + first;
          }
          else
          {
          second = second + "/" + first;
          }
        }
      if (error==true)
        {
        printBL();
        printLN("ERROR. Excepted: COPY FILE DESTINY");
        printBL();
        }
        else
        {
        printBL();
        printLN(first);
        printBL();
        commandCopyProcess(first,second,path);
        printBL();
        printLN("1 file copied.");
        printBL();
        }
      }
    }
  }

void commandCopyProcess(String fileFirstString, String fileSecondString, String initialPath)
  {
  SD.begin(SDPIN);
  boolean error;
  error = false;
  File fileFirst;
  File fileSecond;
  fileFirst = SD.open(stringToChar(initialPath + fileFirstString));
  if (fileFirst & !fileFirst.isDirectory())
    {
    }
    else
    {
    error=true;
    }
  
  if (error==false)
    {
    fileSecond = SD.open(stringToChar(fileSecondString), FILE_WRITE);
    if (fileSecond & !fileSecond.isDirectory())
      {
      while (fileFirst.available())
        {
        fileSecond.write(fileFirst.read());
        }
      }
      else
      {
      error=true;
      }
    }

  fileFirst.close();
  fileSecond.close();
  if (error==true)
    {
    printBL();
    printLN("ERROR. Check file and destiny.");
    printBL();
    }
  }

void commandReset()
  {
  asm volatile ("  jmp 0");
  }

void commandDel(String a)
  {
  if (a=="del" | a=="del ")
    {
    printBL();
    printLN("You must input a file.");
    printBL();
    }
  if (a.substring(0,4)=="del " & a.length()>5)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(4,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(4,a.length()) + "/";
      }
    if (checkIfFileExists(a)==true)
      {
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      if (SD.remove(charBuf)==false)
        {
        printBL();
        printLN("Error. The file can't be deleted.");
        printBL();
        }
      }
      else
      {
      printBL();
      printLN("The file doesn't exist.");
      printBL();
      }
    }
  }

void commandDir()
  {
  SD.begin(SDPIN);
  File root;
  Sd2Card card;
  pinMode(SDPIN, OUTPUT);
  if (!card.init(SPI_HALF_SPEED, SDChipSelect))
    {
    printBL();
    printLN("No SD drive detected.");
    printBL();
    }
    else
    {
    SD.begin(SDPIN);
    char charBuf[50];
    path.toCharArray(charBuf, 50);
    root = SD.open(charBuf);
    root.flush();
    printDirectory(root, 0);
    }
  }

void commandType(String a)
  {
  if (a=="type" | a=="type ")
    {
    printBL();
    printLN("You must input a file.");
    printBL();
    }
  if (a.substring(0,5)=="type " & a.length()>5)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(5,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(5,a.length()) + "/";
      }
    if (checkIfFileExists(a)==true)
      {
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      File myFile;      
      myFile = SD.open(charBuf);
      if (myFile)
        {
        while (myFile.available())
          {
          Serial.write(myFile.read());
          }
        myFile.close();      
        }
      }
      else
      {
      printBL();
      printLN("The file doesn't exists.");
      printBL();
      }
    }
  }

void commandPlay(String a)
  {
  if (a=="play" | a=="play ")
    {
    printBL();
    printLN("You must input a file.");
    printBL();
    }
  if (a.substring(0,5)=="play " & a.length()>5)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(5,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(5,a.length()) + "/";
      }
    if (checkIfFileExists(a)==true)
      {
      TMRpcm tmrpcm;
      tmrpcm.speakerPin = 6;
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      tmrpcm.volume(1);
      tmrpcm.volume(1);
      tmrpcm.play(charBuf);
      printBL();
      printLN("Playing...");
      printBL();
      while (tmrpcm.isPlaying()==1)
        {
        }
      }
      else
      {
      printBL();
      printLN("The file doesn't exists.");
      printBL();
      }
    }
  }

void commandVer()
  {
  printBL();
  printLN("ArduinOS 1.1 by Leonardo Russo");
  printBL();
  }

void commandCls()
  {
  clearScreen();
  }
  
void commandCd(String a)
  {
  if (a=="cd" | a=="cd ")
    {
    printBL();
    printLN("You must input a path.");
    printBL();
    }
  if (a=="cd..")
    {
    String initialPath;
    initialPath = path;
    if (initialPath.length()>root.length())
      {
      initialPath = initialPath.substring(0,initialPath.length()-1);
      path = path.substring(0,initialPath.lastIndexOf("/")+1);
      }
    }
  if (a.substring(0,3)=="cd " & a.length()>3)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(3,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(3,a.length()) + "/";
      }
    if (checkIfDirExists(a)==true)
      {
      path = a;
      }
      else
      {
      printBL();
      printLN("The folder doesn't exists.");
      printBL();
      }
    }
  }

void commandMd(String a)
  {
  if (a=="md" | a=="md ")
    {
    printBL();
    printLN("You must input a name.");
    printBL();
    }
  if (a.substring(0,3)=="md " & a.length()>3)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(3,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(3,a.length()) + "/";
      }
    if (checkIfDirExists(a)==true)
      {
      printBL();
      printLN("The folder already exists.");
      printBL();
      }
      else
      {
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      if (SD.mkdir(charBuf)==false)
        {
        printBL();
        printLN("Error. The folder can't be created.");
        printBL();
        }
      }
    }
  }

void commandRd(String a)
  {
  if (a=="rd" | a=="rd ")
    {
    printBL();
    printLN("You must input a name.");
    printBL();
    }
  if (a.substring(0,3)=="rd " & a.length()>3)
    {
    if (path.length()>root.length())
      {
      a = path + a.substring(3,a.length()) + "/";
      }
      else
      {
      a= root + a.substring(3,a.length()) + "/";
      }
    if (checkIfDirExists(a)==true)
      {
      char charBuf[100];
      a.toCharArray(charBuf, 100);
      if (SD.rmdir(charBuf)==false)
        {
        printBL();
        printLN("Error. The folder can't be deleted.");
        printBL();
        }
      }
      else
      {
      printBL();
      printLN("The folder doesn't exists.");
      printBL();
      }
    }
  }
  
void commandMem()
  {
  printBL();
  printLN("Total memory: ");
  extern int __heap_start, *__brkval; 
  int a;
  printLN(String((int) &a - (__brkval != 0 ? (int) &__heap_start : (int) __brkval)) + " bytes");
  printBL();
  printLN("Free memory: ");
  extern int __heap_start, *__brkval; 
  int v;
  printLN(String((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval)) + " bytes");
  printBL();
  }

void createRootFolder()
  {
  SD.begin(SDPIN);
  String a;
  char charBuf[100];
  a = root;
  a.toCharArray(charBuf, 100);
  SD.mkdir(charBuf);
  }

int count_spaces(String a)
  {
  int i;
  int count;
  count = 0;
  for (i=0;i<a.length()+1;i++)
    {
    if (a.charAt(i)==' ')
      {
      count++;
      }
    }
  return count;
  }

char* stringToChar(String a)
  {
  char charBuf[100];
  a.toCharArray(charBuf, 100);
  return charBuf;
  }

String getPath()
  {
  String pathsystem;
  pathsystem = path;
  pathsystem = pathsystem.substring(7,pathsystem.length());
  if (pathsystem.length()>1)
    {
    pathsystem = pathsystem.substring(0,pathsystem.length()-1);
    }
  pathsystem.replace("/", "\\");
  pathsystem.toUpperCase();
  return pathsystem;
  }

boolean checkIfDirExists(String a)
  {
  SD.begin(SDPIN);
  File dirFile;
  dirFile = SD.open(stringToChar(a), O_READ);
  if (!dirFile.isDirectory())
    {
    dirFile.close();
    return false;
    }
    else
    {
    dirFile.close();
    return true;
    }
  }

boolean checkIfFileExists(String a)
  {
  SD.begin(SDPIN);
  File dirFile;
  dirFile = SD.open(stringToChar(a), O_READ);
  if (!dirFile | dirFile.isDirectory())
    {
    dirFile.close();
    return false;
    }
    else
    {
    dirFile.close();
    return true;
    }
  }

void printDirectory(File dir, int numTabs)
  {
  SD.begin(SDPIN);
  printBL();
  printLN("Directory from A:" + getPath());
  printBL();
  printBL();
  if (path.length()>root.length())
    {
    printLN(".               <DIR>");
    printBL();
    printLN("..              <DIR>");
    printBL();
    }
  long filesizes = 0;
  while(true)
    {
    File entry =  dir.openNextFile();
    if (!entry)
      {
      break;
      }
    String value;
    value = entry.name();
    printLN(entry.name());
    if (value.length()<13)
      {
      int i;
      for (i=0;i<12-value.length();i++)
        {
        printLN(" ");
        }
      }
      
    printLN("    ");

    if (entry.isDirectory())
      {
      printLN("<DIR>");
      printBL();
      }
      else
      {
      String val;
      val = String(entry.size(), DEC);
      printLN(val);
      printBL();
      filesizes = filesizes + atol(stringToChar(val));
      }
    entry.close();
    }
  printLN("                " + String(filesizes) + " bytes");
  printBL();

  Sd2Card card;
  SdVolume volume;
  volume.init(card);
  uint64_t totalSpace = volume.clusterCount()*volume.blocksPerCluster()*512;
  uint64_t occupiedSpace = (card.cardSize()*512) - totalSpace;
  printLN("                " + String(long((totalSpace - occupiedSpace) / 1024) / 1024) + " MB free");
  printBL();
  //printLN("                " + String(long((totalSpace) / 1024) / 1024) + " MB total");
  //printBL();
  }
