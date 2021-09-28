# 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
void init_sms()
{
    Serial.println("AT+CMGF=1");
    delay(400);
    Serial.println("AT+CMGS=\"+ZZXXXXXXXXXX\""); // use your 10 digit cell no. here
    delay(400);
}

void send_sms()
{
    Serial.write(26); //ASCII is 26 and meaning is END of LINE
}

void gsm_init()
{
    Serial.println("Finding Module..");
    boolean at_flag = 1;
    while (at_flag)
    {
        Serial.println("AT");
        delay(1);
        while (Serial.available() > 0)
        {
            if (Serial.find("OK"))
                at_flag = 0;
        }

        delay(1000);
    }
    Serial.println("Module Connected..");
    delay(1000);
    Serial.println("Disabling ECHO");
    boolean echo_flag = 1;
    while (echo_flag)
    {
        Serial.println("ATE0");
        while (Serial.available() > 0)
        {
            if (Serial.find("OK"))
                echo_flag = 0;
        }
        delay(1000);
    }
    Serial.println("Echo OFF");
    delay(1000);
    Serial.println("Finding Network..");
    boolean net_flag = 1;
    while (net_flag)
    {
        Serial.println("AT+CPIN?");
        while (Serial.available() > 0)
        {
            if (Serial.find("+CPIN: READY"))
                net_flag = 0;
        }
        delay(1000);
    }
    Serial.println("Network Found..");

    delay(1000);
}
# 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
# 2 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino" 2
# 3 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino" 2
TinyGPSPlus gps;
SoftwareSerial sim(3, 2);
SoftwareSerial ss(4, 5);
int _timeout;
String _buffer;
String number = "+ZZXXXXXXXXXX"; //-> change with your number
float longitude;
float latitude;
String rSMS;
int index = 0;
void setup()
{
  delay(5000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println(" GSM System Started...");
  ss.begin(9600);
  sim.begin(115200);
  delay(1000);
  RecieveMessage();
}
void loop()
{

  if (sim.available() > 0)
  {
    //    Serial.write(sim.read());
    rSMS = sim.readString();
    Serial.println(rSMS);
    if (rSMS.indexOf("#Locate") != -1)
    {
      gpsLocation();
      SendMessage();
      RecieveMessage();
    }
  }
}

void RecieveMessage()
{

  Serial.println("GSM Read an SMS\n");
  delay(1000);
  sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write("Unread Message done\n");
}

void gpsLocation()
{
  ss.begin(9600);
  ss.listen();
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
    {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      latitude = gps.location.lat();
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
      longitude = gps.location.lng();
    }
  }
}

void SendMessage()
{

  sim.listen();
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "Hi, current location of bike is on Latitude = " + String(latitude, 6) + " and Longitude = " + String(longitude, 6);
  sim.println(SMS);
  delay(100);
  sim.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}

String _readSerial()
{
  _timeout = 0;
  while (!sim.available() && _timeout < 12000)
  {
    delay(13);
    _timeout++;
  }
  if (sim.available())
  {
    return sim.readString();
  }
}
