#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define TJA_RESET_ERR "ERROR: TJA1101 not reset correctly\r\n"
#define TJA_NOT_STBY "ERROR: TJA1101 not in standby mode\r\n"
#define TJA_MASTER_MODE "TJA1101 Master mode\r\n"
#define TJA_SLAVE_MODE "TJA1101 Slave mode\r\n"
#define REINIT_TJA "TJA1101 re-init\r\n"
#define OK_TJA "TJA1101 con OK\r\n"
#define FAIL_TJA "TJA1101 con FAIL\r\n"
#define INIT_DONE_OK "Initialization done OK\r\n"


// SMI registers
#define SMI_CNTRL (uint8_t)0
#define SMI_STATUS (uint8_t)1
#define SMI_PHY_ID1 (uint8_t)2
#define SMI_PHY_ID2 (uint8_t)3
#define SMI_AUTO_NEG_ADV (uint8_t)4
#define SMI_AUTO_NEG_LNK (uint8_t)5
#define SMI_AUTO_NEG_EXP (uint8_t)6
#define SMI_AUTO_NEG_PNR (uint8_t)7
#define SMI_REG8 (uint8_t)8
#define SMI_REG9 (uint8_t)9
#define SMI_REG10 (uint8_t)10
#define SMI_REG11 (uint8_t)11
#define SMI_REG12 (uint8_t)12
#define SMI_REG13 (uint8_t)13
#define SMI_REG14 (uint8_t)14
#define SMI_EXT_STAT (uint8_t)15    // TJA1100
#define SMI_AUTO_MDIX (uint8_t)16   
#define SMI_MODE_CNTRL (uint8_t)17  
#define SMI_SPEC_MODE (uint8_t)18   // LAN8700
#define SMI_REG19 (uint8_t)19
#define SMI_REG24 (uint8_t)24
#define SMI_REG25 (uint8_t)25
#define SMI_INTREG (uint8_t)26
#define SMI_AUXMODE4 (uint8_t)26    
#define SMI_STAT_IND (uint8_t)27    // LAN8700
#define SMI_REG28 (uint8_t)28       // LAN8700
#define SMI_INT_SRC_LN (uint8_t)29  // LAN8700
#define SMI_INT_MSK (uint8_t)30     // LAN8700
#define SMI_PHY_CNTRL (uint8_t)31   /

#define SMI_EXT_CNTRL 17    // 0x11 TJA1100
#define SMI_CONFIG1 18      // 0x12 TJA1100
#define SMI_CONFIG2 19      // 0x13 TJA1100
#define SMI_SYM_ERR 20      // 0x14 TJA1100
#define SMI_INT_SRC_TJA 21  // 0x15 TJA1100
#define SMI_INT_ENA_TJA 22  // 0x16 TJA1100
#define SMI_COMM_STAT 23    // 0x17 TJA1100
#define SMI_GEN_STAT 24     // 0x18 TJA1100
#define SMI_EXTERN_STAT 25  // 0x19 TJA1100
#define SMI_LINK_FAIL 26    // 0x1A TJA1100
#define SMI_COM_CONF 27     // 0x1B TJA1101

// basic control register 0
#define SMI_0_RESET (1 << 15)
#define SMI_0_LOOPBACK (1 << 14)
#define SMI_0_SPD_10 ((0 << 13) | (0 << 6))
#define SMI_0_SPD_100 ((1 << 13) | (0 << 6))
#define SMI_0_SPD_1000 ((0 << 13) | (1 << 6))
#define SMI_0_SPD_MASK ((1 << 13) | (1 << 6))
#define SMI_0_AUTONEG_EN (1 << 12)  // Not supported by TJA1100
#define SMI_0_POWER_DOWN (1 << 11)  // ISOLATE should be zero
#define SMI_0_ISOLATE (1 << 10)     // should be off for normal operation
#define SMI_0_RE_AUTO (1 << 9)      // Not supported by TJA1100
#define SMI_0_DUPLEX (1 << 8)       // Only supported by TJA1100
#define SMI_0_COLL_TST (1 << 7)     // Not supported by TJA1100
#define SMI_0_UNIDIR (1 << 5)

// basic status register 1
#define SMI_1_100BASE_T4 (1 << 15)
#define SMI_1_100BASE_X_FD (1 << 14)
#define SMI_1_100BASE_X_HD (1 << 13)
#define SMI_1_10MBPS_FD (1 << 12)
#define SMI_1_10MBPS_HD (1 << 11)
#define SMI_1_100BASE_T2_FD (1 << 10)
#define SMI_1_100BASE_T2_HD (1 << 9)
#define SMI_1_EXT_STAT (1 << 8)
#define SMI_1_UNI_ABLE (1 << 7)
#define SMI_1_MF_PRE_SUP (1 << 6)
#define SMI_1_AUTONEG_CMP (1 << 5)
#define SMI_1_REM_FAULT (1 << 4)
#define SMI_1_AUTONEG_ABLE (1 << 3)
#define SMI_1_LINK_STAT (1 << 2)
#define SMI_1_JAB_DET (1 << 1)
#define SMI_1_EXT_CAP (1 << 0)

// PHY identifier register 1
#define SMI_2_TJA1101 0x0180
#define SMI_2_BCM5241 0x0143

// PHY identifier register 2
#define SMI_3_TJA1101 0xDD01
#define SMI_3_BCM5241 0xBC31

// PHY SMI_EXT_CNTRL TJA1100
#define SMI_17_LINK_CNTRL (1 << 15)
#define SMI_17_NOCHANGE (0x0 << 11)
#define SMI_17_NORMAL (0x3 << 11)
#define SMI_17_STBY (0xC << 11)
#define SMI_17_SLEEP_REQ (0xB << 11)
#define SMI_17_PWR_MASK (0xF << 11)
#define SMI_17_TRAIN (1 << 9)
#define SMI_17_TEST1 (1 << 6)
#define SMI_17_TEST2 (2 << 6)
#define SMI_17_TEST3 (3 << 6)
#define SMI_17_TEST4 (4 << 6)
#define SMI_17_TEST5 (5 << 6)
#define SMI_17_TEST6 (6 << 6)
#define SMI_17_TEST7 (7 << 6)
#define SMI_17_CABLE_TST (1 << 5)
#define SMI_17_INT_LPB (0 << 3)
#define SMI_17_EXT1_LPB (1 << 3)
#define SMI_17_EXT2_LPB (2 << 3)
#define SMI_17_REM_LPB (3 << 3)
#define SMI_17_CONFIG_EN (1 << 2)
#define SMI_17_CONFIG_INH (1 << 1)
#define SMI_17_WAKE_REQ (1 << 0)  // transmit idle symbols as bus wake-up request

// PHY SMI_CONFIG1  TJA1100
#define SMI_18_MASTER (1 << 15)
#define SMI_18_AUTO_OP (1 << 14)
#define SMI_18_LINK_15M (1 << 13)  // cable length > 15 m
#define SMI_18_TX_500MV (0 << 10)
#define SMI_18_TX_750MV (1 << 10)
#define SMI_18_TX_1000MV (2 << 10)
#define SMI_18_TX_1250MV (3 << 10)
#define SMI_18_MII_MODE (0 << 8)
#define SMI_18_RMII_50MHZ (1 << 8)
#define SMI_18_RMII_25MHZ (2 << 8)
#define SMI_18_REV_MII (3 << 8)
#define SMI_18_MII_DRV_RED (1 << 7)  // reduced strength MII output driver
#define SMI_18_LEDLINK (0 << 4)
#define SMI_18_LEDFRAME (1 << 4)
#define SMI_18_LEDSYMERR (2 << 4)
#define SMI_18_LEDCRS (3 << 4)
#define SMI_18_LED_EN (1 << 3)
#define SMI_18_CNFG_WAKE (1 << 2)  // ratiometric input threshold, absolute if zero
#define SMI_18_AUTO_PWD (1 << 1)   // autonomous power-down enabled

// PHY SMI CONFIG2 TJA1100
#define SMI_19_PHYAD_MASK (0x1F << 11)  // readback of scrambler key
#define SMI_19_SNR_AV32 (0 << 9)        // signal to noise ratio averaging over  32 symbols
#define SMI_19_SNR_AV64 (1 << 9)        // signal to noise ratio averaging over  64 symbols
#define SMI_19_SNR_AV128 (2 << 9)       // signal to noise ratio averaging over 128 symbols
#define SMI_19_SNR_AV256 (3 << 9)       // signal to noise ratio averaging over 256 symbols
#define SMI_19_WLIM_NO (0 << 6)         // no warning
#define SMI_19_WLIM_A (1 << 6)          // Class A SNR warning limit
#define SMI_19_WLIM_B (2 << 6)          // Class B SNR warning limit
#define SMI_19_WLIM_C (3 << 6)          // Class C SNR warning limit
#define SMI_19_WLIM_D (4 << 6)          // Class D SNR warning limit
#define SMI_19_WLIM_E (5 << 6)          // Class E SNR warning limit
#define SMI_19_WLIM_F (6 << 6)          // Class F SNR warning limit
#define SMI_19_WLIM_G (7 << 6)          // Class G SNR warning limit
#define SMI_19_SNR_F_NL (0 << 3)        // signal to noise ratio fail limit - no limit
#define SMI_19_SNR_F_CLA (1 << 3)       // signal to noise ratio fail limit - Class A
#define SMI_19_SNR_F_CLB (2 << 3)       // signal to noise ratio fail limit - Class B
#define SMI_19_SNR_F_CLC (3 << 3)       // signal to noise ratio fail limit - Class C
#define SMI_19_SNR_F_CLD (4 << 3)       // signal to noise ratio fail limit - Class D
#define SMI_19_SNR_F_CLE (5 << 3)       // signal to noise ratio fail limit - Class E
#define SMI_19_SNR_F_CLF (6 << 3)       // signal to noise ratio fail limit - Class F
#define SMI_19_SNR_F_CLG (7 << 3)       // signal to noise ratio fail limit - Class G
#define SMI_19_JUMBO_EN (1 << 2)        // enable packets up to 16 kB instead of 4 kB
#define SMI_19_SLP_T_04 (0 << 0)        // sleep request timeout 0.4 ms
#define SMI_19_SLP_T_1 (1 << 0)         // sleep request timeout 1 ms
#define SMI_19_SLP_T_4 (2 << 0)         // sleep request timeout 4 ms
#define SMI_19_SLP_T_16 (3 << 0)        // sleep request timeout 16 ms

// PHY SMI COM STAT TJA1100
#define SMI_23_LINK_STAT (1 << 15)     // Link OK
#define SMI_23_TX_MODE_MASK (3 << 13)  // TX mode mask
#define SMI_23_TX_MODE_DIS (0 << 13)   // transmitter disabled
#define SMI_23_TX_MODE_N (1 << 13)     // transmitter in SEND_N mode
#define SMI_23_TX_MODE_I (2 << 13)     // transmitter in SEND_I mode
#define SMI_23_TX_MODE_Z (3 << 13)     // transmitter in SEND_Z mode
#define SMI_23_LOC_RCVR_ST (1 << 12)   // local receiver OK
#define SMI_23_REM_RCVR_ST (1 << 11)   // remote receiver OK
#define SMI_23_SCR_LOCKED (1 << 10)    // descrambler locked
#define SMI_23_SSD_ERR (1 << 9)        // SSD error detected
#define SMI_23_ESD_ERR (1 << 8)        // ESD error detected
#define SMI_23_SNR_MASK (7 << 5)       // Signal to noise mask
#define SMI_23_SNR_F_WRS (0 << 5)      // worse than Class A
#define SMI_23_SNR_F_CLA (1 << 5)      // Class A
#define SMI_23_SNR_F_CLB (2 << 5)      // Class B
#define SMI_23_SNR_F_CLC (3 << 5)      // Class C
#define SMI_23_SNR_F_CLD (4 << 5)      // Class D
#define SMI_23_SNR_F_CLE (5 << 5)      // Class E
#define SMI_23_SNR_F_CLF (6 << 5)      // Class F
#define SMI_23_SNR_F_CLG (7 << 5)      // Class G
#define SMI_23_REC_NO_ERR (1 << 4)     // no receive error detected
#define SMI_23_TRAN_NO_ERR (1 << 3)    // no transmitt error detected
#define SMI_23_PHY_STAT_MSK (7 << 0)   // phy state mask
#define SMI_23_PHY_IDLE (0 << 0)       // idle
#define SMI_23_PHY_INIT (1 << 0)       // initialized
#define SMI_23_PHY_CONFIG (2 << 0)     // configured
#define SMI_23_PHY_OFFLINE (3 << 0)    // Offline
#define SMI_23_PHY_ACTIVE (4 << 0)     // Active
#define SMI_23_PHY_ISOLATE (5 << 0)    // isolate
#define SMI_23_PHY_CABLE (6 << 0)      // cable test
#define SMI_23_PHY_TEST (7 << 0)       // test mode


#define MDCIP D5  // wiringPi PIN for MDCIP
#define MDIO D6   // wiringPi PIN for MDIO

byte add_rtl = 0b00010;
byte add_tja = 0b00100;


int smi_init(void) {
  pinMode(MDCIP, OUTPUT);
  pinMode(MDIO, OUTPUT);
  return 0;
}


/*! Generates a rising edge pulse on IP175 MDC */
void pulse_mdcip(void) {
  digitalWrite(MDCIP, 0);
  delay(1);
  digitalWrite(MDCIP, 1);
  delay(1);
}


/*! Performs a smi write */
void write_smi(uint8_t phy, uint8_t reg, uint16_t data) {
  uint8_t byte;
  uint16_t word;

  void (*pulse_mdc)(void);
  pulse_mdc = pulse_mdcip;

  /* MDIO pin is output */
  pinMode(MDIO, OUTPUT);

  digitalWrite(MDIO, 1);
  for (byte = 0; byte < 32; byte++)
    pulse_mdc();

  /* Stat code */
  digitalWrite(MDIO, 0);
  pulse_mdc();
  digitalWrite(MDIO, 1);
  pulse_mdc();

  /* Write OP Code */
  digitalWrite(MDIO, 0);
  pulse_mdc();
  digitalWrite(MDIO, 1);
  pulse_mdc();

  /* PHY address - 5 bits */
  for (byte = 0x10; byte != 0; byte = byte >> 1) {
    if (byte & phy)
      digitalWrite(MDIO, 1);
    else
      digitalWrite(MDIO, 0);
    pulse_mdc();
  }
  /* REG address - 5 bits */
  for (byte = 0x10; byte != 0; byte = byte >> 1) {
    if (byte & reg)
      digitalWrite(MDIO, 1);
    else
      digitalWrite(MDIO, 0);

    pulse_mdc();
  }
  /* Turn around bits */
  digitalWrite(MDIO, 1);
  pulse_mdc();
  digitalWrite(MDIO, 0);
  pulse_mdc();

  /* Data - 16 bits */
  for (word = 0x8000; word != 0; word = word >> 1) {
    if (word & data)
      digitalWrite(MDIO, 1);
    else
      digitalWrite(MDIO, 0);

    pulse_mdc();
  }

  /* This is needed for some reason... */
  pulse_mdc();
  /* Stay in 0 state */
  //  MDCIP = 0;
}

uint16_t read_smi(uint8_t phy, uint8_t reg) {
  uint8_t byte;
  uint16_t word, data;

  void (*pulse_mdc)(void);

  pulse_mdc = pulse_mdcip;

  /* MDIO pin is output */
  pinMode(MDIO, OUTPUT);

  digitalWrite(MDIO, 1);
  for (byte = 0; byte < 32; byte++)
    pulse_mdc();

  /* Stat code */
  digitalWrite(MDIO, 0);
  pulse_mdc();
  digitalWrite(MDIO, 1);
  pulse_mdc();

  /* Read OP Code */
  digitalWrite(MDIO, 1);
  pulse_mdc();
  digitalWrite(MDIO, 0);
  pulse_mdc();

  /* PHY address - 5 bits */
  for (byte = 0x10; byte != 0;) {
    if (byte & phy)
      digitalWrite(MDIO, 1);
    else
      digitalWrite(MDIO, 0);

    pulse_mdc();

    byte = byte >> 1;
  }
  /* REG address - 5 bits */
  for (byte = 0x10; byte != 0;) {
    if (byte & reg)
      digitalWrite(MDIO, 1);
    else
      digitalWrite(MDIO, 0);

    pulse_mdc();

    byte = byte >> 1;
  }
  /* Turn around bits */
  /* MDIO now is input */
  pinMode(MDIO, INPUT_PULLUP);
  pulse_mdc();
  pulse_mdc();
  /* Data - 16 bits */
  data = 0;
  for (word = 0x8000; word != 0;) {

    if (digitalRead(MDIO))
      data |= word;

    pulse_mdc();


    word = word >> 1;
  }

  /* This is needed for some reason... */
  pulse_mdc();
  /* Stay in 0 state */
  //  MDCIP = 0;

  return data;
}

void binprintf(int v) {
  unsigned int mask = 1 << ((sizeof(uint16_t) << 3) - 1);
  while (mask) {
    printf("%d", (v & mask ? 1 : 0));
    mask >>= 1;
  }
}

void setup() {
  uint16_t reg_val = 0;
  // start serial port at 9600 bps:
  Serial.begin(115200);
  smi_init();
  delay(6000);
  Serial.print("Ready...\n\n\n\n");
  reg_val = read_smi(2, 0x00);
  Serial.print("reg_val=[0x");
  Serial.print(reg_val, HEX);
  Serial.print("]\n");


  uint16_t val;
  write_smi(add_tja, SMI_CNTRL, SMI_0_RESET);  // send reset to TJA1101
  val = read_smi(add_tja, SMI_CNTRL);
  if (!(((val & SMI_0_SPD_MASK) == SMI_0_SPD_100) && (val & SMI_0_DUPLEX))) {
    printf(TJA_RESET_ERR);
  }

  printregister(add_tja, SMI_INT_SRC_TJA);              // read interrupt source
  // printf(" SMI_INT_SRC_TJA");
  printregister(add_tja, SMI_INT_SRC_TJA);              // read interrupt source
  // printf(" SMI_INT_SRC_TJA");
  printregister(add_tja, SMI_INT_SRC_TJA);              // read interrupt source
  // printf(" SMI_INT_SRC_TJA");
  // printf("\nEnabling configuration on reg 17 BIT 2.....\n");
  printregister(add_tja, SMI_EXT_CNTRL);                // read interrupt source
  // printf(" chech config disabled... SMI_EXT_CNTRL");
  write_smi(add_tja, SMI_EXT_CNTRL, SMI_17_CONFIG_EN);  // select mode TJA1101
  printregister(add_tja, SMI_EXT_CNTRL);                // read interrupt source
  // printf(" SMI_EXT_CNTRL");
  // printf("\nIs configuration enabled??\n");
  // write_smi(add_tja, SMI_CONFIG1, (SMI_18_REV_MII | (1 << 7)));

  printregister(add_tja, SMI_CONFIG1);

  // write_smi(add_tja, SMI_CNTRL, SMI_0_LOOPBACK);  // loopback
  // write_smi(add_tja, SMI_EXT_CNTRL, ((0x2 << 3) | SMI_17_CONFIG_EN));


  // printf(" SMI_CONFIG1");
  write_smi(add_tja, SMI_COM_CONF, (1 << 15));                                                // fix TJA1101

  delay(80);                                                                                  // delay about 10 ms
  write_smi(add_tja, SMI_EXT_CNTRL, (SMI_17_LINK_CNTRL | SMI_17_NORMAL | SMI_17_CONFIG_EN));  // select normal mode TJA1101

  printregister(add_tja, SMI_EXT_CNTRL);  // read interrupt source
                                          //  printf(" SMI_EXT_CNTRL");
  /*
  */
  write_smi(add_rtl, SMI_CNTRL, SMI_0_RESET);  // send reset
  write_smi(add_rtl, 31, (0x7 << 0));          //Set page 7

  delay(80);
  write_smi(add_rtl, 16, 0b0000111111110010);
  // write_smi(add_rtl, SMI_CNTRL, SMI_0_LOOPBACK);  // loopback rtl
  // write_smi(add_rtl, SMI_REG28,  ((0 << 1) | (0 << 2)));  // loopback rtl
  write_smi(add_rtl, SMI_CNTRL, (SMI_0_AUTONEG_EN));
  //write_smi(add_rtl, SMI_CNTRL, 0b0010000100000000);
}

void printregister(uint8_t add, uint8_t reg) {
  uint16_t reg_val = read_smi(add, reg);
  if (add == add_tja) {
    printf("\nTJA1101B,Register:%2d,Value:", reg);
  } else {
    printf("\nRTL8201F,Register:%2d,Value:", reg);
  }
  binprintf(reg_val);
}


void loop() {
  uint16_t reg_val = 0;

  delay(1000);

  printregister(add_rtl, 0);
  // printf(" Basic Mode Control Register ");
  printregister(add_rtl, 1);
  // printf(" Basic Mode Status Registerr ");
  printregister(add_rtl, 4);
  // printf(" Auto-Negotiation Advertisement register ");
  printregister(add_rtl, 5);
  // printf(" Auto-Negotiation Link parter ability register ");
  printregister(add_rtl, 6);
  // printf(" Auto-Negotiation Expansion register ");
  // printregister(add_rtl, 23);
  // printf(" Force Link and Bypass Scrambler register ");
  // printregister(add_rtl, 24);
  // printf(" Receiver Error Mask register ");
  // printregister(add_rtl, 28);
  // printf(" Fiber Mode and Loopback register ");
  // printregister(add_rtl, 30);
  // printf(" SNR Display register ");
  // printregister(add_rtl, 31);
  //  printf(" Page Select register ");
  printregister(add_rtl, 16);
  printf(" RMII Mode Setting register ");
  // printregister(add_rtl, 17);
  // printf(" Customized LED settings register ");
  // printregister(add_rtl, 18);
  // printf(" Led Enable register ");
  printregister(add_rtl, 19);
  printf(" Package indicator & LED Function register ");
  // printregister(add_rtl, 20);
  // printf(" MII TX Isolate register ");
  printregister(add_rtl, 31);
  printf(" Register 31 Page Select Register  ");

  printregister(add_tja, 0);
  // printf(" Basic control register ");
  printregister(add_tja, 1);
  // printf(" Basic status register ");
  printregister(add_tja, 15);
  // printf(" Extended status register ");
  printregister(add_tja, 17);
  // printf(" Extended control register ");
  printregister(add_tja, 18);
  // printf(" Configuration register 1 ");
  printregister(add_tja, 19);
  // printf(" Configuration register 2 ");
  printregister(add_tja, 20);
  // printf(" Symbol error counter register ");
  printregister(add_tja, 21);
  // printf(" Interrupt source register ");
  printregister(add_tja, 22);
  // printf(" Interrupt source register ");
  printregister(add_tja, 23);
  // printf(" Communication status register ");
  printregister(add_tja, 24);
  // printf(" General status register ");
  printregister(add_tja, 25);
  // printf(" External status register ");
  printregister(add_tja, 26);
  // printf(" Link fail counter register ");
  printregister(add_tja, 27);
  // printf(" Common configuration register ");
  printregister(add_tja, 28);
  // printf(" Configuration register 3 ");/* */
}
