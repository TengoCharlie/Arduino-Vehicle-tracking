void init_sms()
{
    Serial.println("AT+CMGF=1");
    delay(400);
    Serial.println("AT+CMGS=\"+917690028089\""); // use your 10 digit cell no. here
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
