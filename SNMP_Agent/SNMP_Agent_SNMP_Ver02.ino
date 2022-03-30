#include <SPI.h>
#include <Ethernet.h>
#include <Agentuino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

const char adc0[]                 PROGMEM     = "1.3.6.1.2.1.1.10.0";  // Test OID
const char float1[]               PROGMEM     = "1.3.6.1.2.1.1.11.0";  // Test OID

const char sysDescr[]             PROGMEM     = "1.3.6.1.2.1.1.1.0";  // System Description
const char sysContact[]           PROGMEM     = "1.3.6.1.2.1.1.4.0";  // System Contact
const char sysName[]              PROGMEM     = "1.3.6.1.2.1.1.5.0";  // System Name
const char sysLocation[]          PROGMEM     = "1.3.6.1.2.1.1.6.0";  // System Location
const char sysServices[]          PROGMEM     = "1.3.6.1.2.1.1.7.0";  // System Services


//********* Page 01 OIDs *********//
const char oil_pressure[]         PROGMEM     = "1.3.6.1.3.1.1.1.0";     //ADC Value
const char temp[]                 PROGMEM     = "1.3.6.1.3.1.1.2.0";     //Temperature in Celsius
const char fuel_level[]           PROGMEM     = "1.3.6.1.3.1.1.3.0";     //Temperature in Celsius
const char gen_volt[]             PROGMEM     = "1.3.6.1.3.1.1.4.0";     //Temperature in Celsius
const char eng_batt[]             PROGMEM     = "1.3.6.1.3.1.1.5.0";     //Temperature in Celsius
const char eng_speed[]            PROGMEM     = "1.3.6.1.3.1.1.6.0";     //ADC Value
const char gen_freq[]             PROGMEM     = "1.3.6.1.3.1.1.7.0";     //ADC Value
const char L1_N_volt[]            PROGMEM     = "1.3.6.1.3.1.1.8.0";     //Temperature in Celsius
const char L2_N_volt[]            PROGMEM     = "1.3.6.1.3.1.1.9.0";     //Temperature in Celsius
const char L3_N_volt[]            PROGMEM     = "1.3.6.1.3.1.1.10.0";     //Temperature in Celsius
const char L1_L2_volt[]           PROGMEM     = "1.3.6.1.3.1.1.11.0";     //Temperature in Celsius
const char L2_L3_volt[]           PROGMEM     = "1.3.6.1.3.1.1.12.0";     //Temperature in Celsius
const char L3_L1_volt[]           PROGMEM     = "1.3.6.1.3.1.1.13.0";     //Temperature in Celsius
const char L1_current[]           PROGMEM     = "1.3.6.1.3.1.1.14.0";     //Temperature in Celsius
const char L2_current[]           PROGMEM     = "1.3.6.1.3.1.1.15.0";     //Temperature in Celsius
const char L3_current[]           PROGMEM     = "1.3.6.1.3.1.1.16.0";     //Temperature in Celsius
const char L1_watts[]             PROGMEM     = "1.3.6.1.3.1.1.17.0";     //Temperature in Celsius
const char L2_watts[]             PROGMEM     = "1.3.6.1.3.1.1.18.0";     //ADC Value
const char L3_watts[]             PROGMEM     = "1.3.6.1.3.1.1.19.0";     //Temperature in Celsius
const char main_freq[]            PROGMEM     = "1.3.6.1.3.1.1.20.0";     //Temperature in Celsius
const char main_L1_N_volt[]       PROGMEM     = "1.3.6.1.3.1.1.21.0";     //Temperature in Celsius
const char main_L2_N_volt[]       PROGMEM     = "1.3.6.1.3.1.1.22.0";     //Temperature in Celsius
const char main_L3_N_volt[]       PROGMEM     = "1.3.6.1.3.1.1.23.0";     //Temperature in Celsius
const char main_L1_L2_volt[]      PROGMEM     = "1.3.6.1.3.1.1.24.0";     //Temperature in Celsius
const char main_L2_L3_volt[]      PROGMEM     = "1.3.6.1.3.1.1.25.0";     //Temperature in Celsius
const char main_L3_L1_volt[]      PROGMEM     = "1.3.6.1.3.1.1.26.0";     //Temperature in Celsius


//********* Page 02 OIDs *********//
const char gen_tot_watts[]        PROGMEM     = "1.3.6.1.3.1.2.1.0";     //ADC Value


//********* Page 03 OIDs *********//
const char eng_run_time[]         PROGMEM     = "1.3.6.1.3.1.3.1.0";     //ADC Value
const char gen_pos_KWh[]          PROGMEM     = "1.3.6.1.3.1.3.2.0";     //Temperature in Celsius
const char gen_neg_kwh[]          PROGMEM     = "1.3.6.1.3.1.3.3.0";     //ADC Value
const char gen_KVAh[]             PROGMEM     = "1.3.6.1.3.1.3.4.0";     //Temperature in Celsius
const char gen_KVArh[]            PROGMEM     = "1.3.6.1.3.1.3.5.0";     //Temperature in Celsius
const char gen_num_starts[]       PROGMEM     = "1.3.6.1.3.1.3.6.0";     //Temperature in Celsius


//********* Page 04 OID's *********//
const char underspeed[]           PROGMEM     = "1.3.6.1.3.1.4.1.0";     //ADC Value
const char overspeed[]            PROGMEM     = "1.3.6.1.3.1.4.2.0";     //Temperature in Celsius
const char fail_to_start[]        PROGMEM     = "1.3.6.1.3.1.4.3.0";     //ADC Value
const char fail_come_start[]      PROGMEM     = "1.3.6.1.3.1.4.4.0";     //Temperature in Celsius


//********* Custom Sensor's OIDs *********//
const char fuel_sensor[]          PROGMEM     = "1.3.6.1.3.1.5.1.0";     //ADC Value
const char curr_sensor_1[]        PROGMEM     = "1.3.6.1.3.1.5.2.0";     //ADC Value
const char curr_sensor_2[]        PROGMEM     = "1.3.6.1.3.1.5.3.0";     //ADC Value
const char curr_sensor_3[]        PROGMEM     = "1.3.6.1.3.1.5.4.0";     //ADC Value


// RFC1213 local values
static char locDescr[]            = "SNMP GENSET monitoring";                 // read-only (static)
static char locContact[]        = "AKSA-SDS";
static char locName[]           = "ServerRoom";
static char locLocation[]       = "Planet Earth";
static int32_t locServices        = 2;


static byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //Mac Address for Arduino Ethernet Shield
static byte ip[] = { 10, 250, 20, 109 };
//static byte ip[] = { 192, 168, 100, 3 };                    //IP Address for Arduino Ethernet Shield
//static byte gateway[] = { 192, 168, 100, 1 };
//static byte subnet[] = { 255, 255, 255, 0 };

byte inByte[129];

int adc_val = 16;
char float1_val[] = "100.001";
const double fac0_1 = 0.1;
const double fac0_01 = 0.01;
const double fac0_001 = 0.001;

long p1_1, p1_2, p1_3, p1_6;
signed long p1_17, p1_18, p1_19;
char p1_4[10], p1_5[10], p1_7[10], p1_8[10], p1_9[10], p1_10[10], p1_11[10], p1_12[10], p1_13[10], p1_14[12];
char p1_15[12], p1_16[12], p1_20[10], p1_21[10], p1_22[10], p1_23[10], p1_24[10], p1_25[10], p1_26[10];

long p2_1, p3_1, p3_6;
char p3_2[10], p3_3[10], p3_4[10], p3_5[10];

int p4_1, p4_2, p4_3, p4_4;

int p5_1 = 75;
char p5_2[7] = "120.3";
char p5_3[7] = "140.6";
char p5_4[7] = "160.9";

double val;

int j = 0;

char oid[SNMP_MAX_OID_LEN];
SNMP_API_STAT_CODES api_status;
SNMP_ERR_CODES status;

union {
  uint32_t i;
  double f;
} data;

ISR(USART_RX_vect)
{
  if (j >= 133)
    //  if (j >= 129)
    //  if (j >= 126)
    j = 0;
  else
    inByte[j++] = UDR0;

  UDR0 = 0x00;
}

void pduReceived()
{
  SNMP_PDU pdu;
  api_status = Agentuino.requestPdu(&pdu);

  if ((pdu.type == SNMP_PDU_GET || pdu.type == SNMP_PDU_GET_NEXT || pdu.type == SNMP_PDU_SET)
      && pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS ) {

    pdu.OID.toString(oid);

    if ( strcmp_P(oid, sysDescr ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locDescr);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, sysName ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        status = pdu.VALUE.decode(locName, strlen(locName));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locName);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, sysContact ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        status = pdu.VALUE.decode(locContact, strlen(locContact));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locContact);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, sysLocation ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        status = pdu.VALUE.decode(locLocation, strlen(locLocation));
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locLocation);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, sysServices) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locServices);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }

    }
    else if ( strcmp_P(oid, oil_pressure ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_1);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, temp ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_2);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, fuel_level ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_3);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        //        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_4);
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_4);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, eng_batt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_5);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, eng_speed ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        //        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, float1_val);
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_6);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_freq ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_7);
        //        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_7);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L1_N_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_8);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L2_N_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_9);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L3_N_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_10);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L1_L2_volt) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_11);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }

    }
    else if ( strcmp_P(oid, L2_L3_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_12);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L3_L1_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_13);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L1_current ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_14);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L2_current ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_15);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L3_current ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_16);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L1_watts ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_17);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L2_watts ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_18);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, L3_watts ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p1_19);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_freq ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_20);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L1_N_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_21);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L2_N_volt) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_22);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L3_N_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_23);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L1_L2_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_24);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L2_L3_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_25);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, main_L3_L1_volt ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p1_26);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //********* Page 02 OIDs *********//
    else if ( strcmp_P(oid, gen_tot_watts ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p2_1);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //******* Page 03 OIDs ********//
    else if ( strcmp_P(oid, eng_run_time ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p3_1);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_pos_KWh ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p3_2);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_neg_kwh ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p3_3);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_KVAh ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p3_4);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_KVArh ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p3_5);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, gen_num_starts ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p3_6);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //********* Page 04 OIDs *********//
    else if ( strcmp_P(oid, underspeed ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p4_1);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, overspeed ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p4_2);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, fail_to_start ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p4_3);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    else if ( strcmp_P(oid, fail_come_start ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p4_4);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //********* Custom Sensor's OIDs *********//
    
    else if ( strcmp_P(oid, fuel_sensor ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, p5_1);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }
    
    else if ( strcmp_P(oid, curr_sensor_1 ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p5_2);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    else if ( strcmp_P(oid, curr_sensor_2 ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p5_3);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    else if ( strcmp_P(oid, curr_sensor_3 ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, p5_4);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //********* ADC Test OID *********//
    else if ( strcmp_P(oid, adc0 ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET )
      {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      }
      else
      {
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, adc_val);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    //********* Float Value Test OID *********//
    else if ( strcmp_P(oid, float1 ) == 0 ) {
      if ( pdu.type == SNMP_PDU_SET ) {
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, float1_val);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
    }

    else {
      pdu.type = SNMP_PDU_RESPONSE;
      pdu.error = SNMP_ERR_NO_SUCH_NAME;
    }

    Agentuino.responsePdu(&pdu);
  }
  Agentuino.freePdu(&pdu);
}

void debug()
{
  mySerial.println(p1_1);
  mySerial.println(p1_2);
  mySerial.println(p1_3);
  mySerial.println(p1_4);
  mySerial.println(p1_5);
  mySerial.println(p1_6);
  mySerial.println(p1_7);
  mySerial.println(p1_8);
  mySerial.println(p1_9);
  mySerial.println(p1_10);
  mySerial.println(p1_11);
  mySerial.println(p1_12);
  mySerial.println(p1_13);
  mySerial.println(p1_14);
  mySerial.println(p1_15);
  mySerial.println(p1_16);
  mySerial.println(p1_17);
  mySerial.println(p1_18);
  mySerial.println(p1_19);
  mySerial.println(p1_20);
  mySerial.println(p1_21);
  mySerial.println(p1_22);
  mySerial.println(p1_23);
  mySerial.println(p1_24);
  mySerial.println(p1_25);
  mySerial.println(p1_26);
  mySerial.println(p2_1);
  mySerial.println(p3_1);
  mySerial.println(p3_2);
  mySerial.println(p3_3);
  mySerial.println(p3_4);
  mySerial.println(p3_5);
  mySerial.println(p3_6);
}

void setup()
{
  //  Serial.begin(9600);

  //  mySerial.begin(9600);
  //  mySerial.println("Starting....");
  //  delay(2000);

  pinMode(4, OUTPUT);
  
  wdt_enable(WDTO_4S);

  UBRR0 = 103; // for configuring baud rate of 9600bps
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Use 8-bit character sizes
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  // Turn on the transmission, reception, and Receive interrupt

  sei();// enable global interrupt

  //  Ethernet.begin(mac);                     //Initialize Ethernet Shield
  Ethernet.begin(mac, ip);                     //Initialize Ethernet Shield
  api_status = Agentuino.begin();              //Begin Snmp agent on Ethernet shield

  if ( api_status == SNMP_API_STAT_SUCCESS ) {
    Agentuino.onPduReceive(pduReceived);
    delay(10);
    return;
  }
  delay(10);
}

void loop()
{
  //  debug();

  wdt_reset();

  Agentuino.listen();
  /*
    unsigned long batt_val = 0;
    batt_val += (unsigned long)inByte[2] << 8;
    batt_val += (unsigned long)inByte[1];

    double batt = (double)batt_val * fac0_1;
    dtostrf(batt, 3, 1, p1_4);
  */

  //******** Page 01 Data *******************//


  p1_1 = ((long)inByte[1] << 8) + ((long)inByte[0]);

  //  p1_2 += ((unsigned long)inByte[3] << 8) + ((unsigned long)inByte[2]);      // Keep

  p1_2 = ((long)inByte[5] << 8) + ((long)inByte[4]);

  p1_3 = ((long)inByte[7] << 8) + ((long)inByte[6]);

  dtostrf((double)(((long)inByte[9] << 8) + ((long)inByte[8])) * fac0_1, 5, 1, p1_4);

  dtostrf((double)(((long)inByte[11] << 8) + ((long)inByte[10])) * fac0_1, 5, 1, p1_5);

  p1_6 = ((long)inByte[13] << 8) + (long)inByte[12];

  //  dtostrf((((long)inByte[13] << 8) + ((long)inByte[12])) * fac0_1, 5, 1, p1_6);

  dtostrf((double)(((long)inByte[15] << 8) + ((long)inByte[14])) * fac0_01, 5, 2, p1_7);

  char str1_8[10];
  sprintf(str1_8, "%x%x%x%x", inByte[19], inByte[18], inByte[17], inByte[16]);
  data.i = strtoul(str1_8, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_8);

  char str1_9[10];
  sprintf(str1_9, "%x%x%x%x", inByte[23], inByte[22], inByte[21], inByte[20]);
  data.i = strtoul(str1_9, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_9);

  char str1_10[10];
  sprintf(str1_10, "%x%x%x%x", inByte[27], inByte[26], inByte[25], inByte[24]);
  data.i = strtoul(str1_10, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_10);

  char str1_11[10];
  sprintf(str1_11, "%x%x%x%x", inByte[31], inByte[30], inByte[29], inByte[28]);
  data.i = strtoul(str1_11, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_11);

  char str1_12[10];
  sprintf(str1_12, "%x%x%x%x", inByte[35], inByte[34], inByte[33], inByte[32]);
  data.i = strtoul(str1_12, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_12);


  char str1_13[10];
  sprintf(str1_13, "%x%x%x%x", inByte[39], inByte[38], inByte[37], inByte[36]);
  data.i = strtoul(str1_13, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p1_13);

  char str1_14[10];
  sprintf(str1_14, "%x%x%x%x", inByte[43], inByte[42], inByte[41], inByte[40]);
  data.i = strtoul(str1_14, NULL, 16);
  val = data.f * fac0_001;
  dtostrf(val, 5, 3, p1_14);

  char str1_15[10];
  sprintf(str1_15, "%x%x%x%x", inByte[47], inByte[46], inByte[45], inByte[44]);
  data.i = strtoul(str1_15, NULL, 16);
  dtostrf(data.f * fac0_001, 5, 3, p1_15);

  char str1_16[10];
  sprintf(str1_16, "%x%x%x%x", inByte[51], inByte[50], inByte[49], inByte[48]);
  data.i = strtoul(str1_16, NULL, 16);
  dtostrf(data.f * fac0_001, 5, 3, p1_16);

  //  p1_17 += ((unsigned long)inByte[53] << 24) + ((unsigned long)inByte[52] << 16);   //keep
  //  p1_17 += ((unsigned long)inByte[55] << 8) + ((unsigned long)inByte[54]);   //keep

  p1_17 = ((signed long)inByte[59] << 24) + ((signed long)inByte[58] << 16);
  p1_17 += ((signed long)inByte[57] << 8) + ((signed long)inByte[56]);

  p1_18 = ((signed long)inByte[63] << 24) + ((signed long)inByte[62] << 16);
  p1_18 += ((signed long)inByte[61] << 8) + ((signed long)inByte[60]);

  p1_19 = ((signed long)inByte[67] << 24) + ((signed long)inByte[66] << 16);
  p1_19 += ((signed long)inByte[65] << 8) + ((signed long)inByte[64]);

  //  p1_20 += ((unsigned long)inByte[69] << 8) + ((unsigned long)inByte[68]);   //keep

  dtostrf((((long)inByte[71] << 8) + ((long)inByte[70])) * fac0_01, 5, 2, p1_20);

  char str1_21[10];
  sprintf(str1_21, "%x%x%x%x", inByte[75], inByte[74], inByte[73], inByte[72]);
  data.i = strtoul(str1_21, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_21);

  char str1_22[10];
  sprintf(str1_22, "%x%x%x%x", inByte[79], inByte[78], inByte[77], inByte[76]);
  data.i = strtoul(str1_22, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_22);

  char str1_23[10];
  sprintf(str1_23, "%x%x%x%x", inByte[83], inByte[82], inByte[81], inByte[80]);
  data.i = strtoul(str1_23, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_23);

  char str1_24[10];
  sprintf(str1_23, "%x%x%x%x", inByte[87], inByte[86], inByte[85], inByte[84]);
  data.i = strtoul(str1_24, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_24);

  char str1_25[10];
  sprintf(str1_25, "%x%x%x%x", inByte[91], inByte[90], inByte[89], inByte[88]);
  data.i = strtoul(str1_25, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_25);

  char str1_26[10];
  sprintf(str1_26, "%x%x%x%x", inByte[95], inByte[94], inByte[93], inByte[92]);
  data.i = strtoul(str1_26, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 2, p1_26);


  //-******** Page 02 Data *******************-//

  p2_1 = ((signed long)inByte[99] << 24) + ((signed long)inByte[98] << 16);
  p2_1 += ((signed long)inByte[97] << 8) + ((signed long)inByte[96]);

  //-******** Page 03 Data *******************-//

  p3_1 = ((signed long)inByte[103] << 24) + ((signed long)inByte[102] << 16);
  p3_1 += ((signed long)inByte[101] << 8) + ((signed long)inByte[100]);

  char str3_2[10];
  sprintf(str3_2, "%x%x%x%x", inByte[107], inByte[106], inByte[105], inByte[104]);
  data.i = strtoul(str3_2, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p3_2);

  char str3_3[10];
  sprintf(str3_2, "%x%x%x%x", inByte[111], inByte[110], inByte[109], inByte[108]);
  data.i = strtoul(str3_3, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p3_3);

  char str3_4[10];
  sprintf(str3_2, "%x%x%x%x", inByte[115], inByte[114], inByte[113], inByte[112]);
  data.i = strtoul(str3_4, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p3_4);

  char str3_5[10];
  sprintf(str3_5, "%x%x%x%x", inByte[119], inByte[118], inByte[117], inByte[116]);
  data.i = strtoul(str3_5, NULL, 16);
  dtostrf(data.f * fac0_1, 5, 1, p3_5);

  p3_6 = ((signed long)inByte[123] << 24) + ((signed long)inByte[122] << 16);
  p3_6 += ((signed long)inByte[121] << 8) + ((signed long)inByte[120]);



  //-******** Page 04 Data *******************-//

  p4_1 = ( inByte[125] >> 4 ) & 0xf;    //top nibble
  p4_2 = inByte[125] & 0xf;             //bottom nibble
  p4_3 = ( inByte[124] >> 4 ) & 0xf;    //top nibble
  p4_4 = inByte[124] & 0xf;             //bottom nibble


  //-******** Custom Sensors Data *******************-//

  p5_1 = (long)inByte[126];

  dtostrf((((long)inByte[128] << 8) + ((long)inByte[127])) * fac0_01, 5, 2, p5_2);
  dtostrf((((long)inByte[130] << 8) + ((long)inByte[129])) * fac0_01, 5, 2, p5_3);
  dtostrf((((long)inByte[132] << 8) + ((long)inByte[131])) * fac0_01, 5, 2, p5_4);

}
