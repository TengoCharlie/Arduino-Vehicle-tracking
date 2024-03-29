#include <Arduino.h>
#line 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
#line 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
void init_sms();
#line 9 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
void send_sms();
#line 14 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
void gsm_init();
#line 13 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void setup();
#line 32 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void loop();
#line 52 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void serialEvent();
#line 68 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void send_data(String message);
#line 74 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void lcd_status();
#line 82 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
void tracking();
#line 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\GSM.ino"
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

#line 1 "c:\\Users\\thegr\\OneDrive\\Desktop\\del1\\Arduino-Vehicle-tracking\\Vehicle-tracking-new.ino"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
int temp = 0, i;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
String stringVal = "";

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("Vehicle Tracking");
  Serial.println("    System      ");
  delay(2000);
  gsm_init();
  Serial.println("AT+CNMI=2,2,0,0,0");
  Serial.println("GPS Initializing");
  Serial.println("  No GPS Range  ");
  delay(2000);
  Serial.println("GPS Range Found");
  Serial.println("GPS is Ready");
  delay(2000);
  Serial.println("System Ready");
  temp = 0;
}

void loop()
{
  serialEvent();

  while (temp)
  {
    while (ss.available() > 0)
    {
      gps.encode(ss.read());
      if (gps.location.isUpdated())
      {
        temp = 0;
        tracking();
      }
      if (!temp)
        break;
    }
  }
  digitalWrite(13, LOW);
}
void serialEvent()
{
  while (Serial.available() > 0)
  {
    if (Serial.find("Track Vehicle"))
    {
      temp = 1;
      break;
    }
    else
    {
      temp = 0;
    }
  }
}

void send_data(String message)
{
  Serial.print(message);
  delay(200);
}

void lcd_status()
{
  Serial.println("Message Sent");
  delay(2000);
  Serial.println("System Ready");
  return;
}

void tracking()
{
  init_sms();
  send_data("Vehicle Tracking Alert:");
  Serial.println(" ");
  send_data("Your Vehicle Current Location is:");
  Serial.println(" ");
  Serial.print("Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print("\n Longitude: ");
  Serial.println(gps.location.lng(), 6);

  // https://www.google.com/maps/@8.2630696,77.3022699,14z
  Serial.print("https://www.google.com/maps/@");
  Serial.print(gps.location.lat(), 6);
  Serial.print(',');
  Serial.print(gps.location.lng(), 6);
  Serial.print(",14z");
  send_sms();
  delay(2000);
}

