#include "cli.h"
#include "bsp_uart.h"
#include "indicate.h"
#include "task.h"

uint8_t dac_value;
uint8_t dac_value_handle;
int dac_handle_max_value = 100;
int dac_max_value = 10;
extern DAC_HandleTypeDef hdac;
#ifdef _USE_HW_CLI

#define CLI_KEY_BACK              0x08
#define CLI_KEY_DEL               0x7F
#define CLI_KEY_ENTER             0x0D
#define CLI_KEY_ESC               0x1B
#define CLI_KEY_LEFT              0x44
#define CLI_KEY_RIGHT             0x43
#define CLI_KEY_UP                0x41
#define CLI_KEY_DOWN              0x42
#define CLI_KEY_HOME              0x31
#define CLI_KEY_END               0x34

#define CLI_PROMPT_STR            "cli# "

#define CLI_ARGS_MAX              32
#define CLI_PRINT_BUF_MAX         256 

uint8_t stop_flag;
enum {
	CLI_RX_IDLE, CLI_RX_SP1, CLI_RX_SP2, CLI_RX_SP3, CLI_RX_SP4,
};

typedef struct {
	char cmd_str[CLI_CMD_NAME_MAX];
	void (*cmd_func)(cli_args_t*);
} cli_cmd_t;

typedef struct {
	uint8_t buf[CLI_LINE_BUF_MAX];
	uint8_t buf_len;
	uint8_t cursor;
	uint8_t count;
} cli_line_t;

typedef struct {
	uint8_t ch;
	uint32_t baud;
	bool is_open;
	bool is_log;
	uint8_t log_ch;
	uint32_t log_baud;
	uint8_t state;
	char print_buffer[CLI_PRINT_BUF_MAX];
	uint16_t argc;
	char *argv[CLI_ARGS_MAX];

	bool hist_line_new;
	int8_t hist_line_i;
	uint8_t hist_line_last;
	uint8_t hist_line_count;

	cli_line_t line_buf[CLI_LINE_HIS_MAX];
	cli_line_t line;

	uint16_t cmd_count;
	cli_cmd_t cmd_list[CLI_CMD_LIST_MAX];
	cli_args_t cmd_args;
} cli_t;

cli_t cli_node;

static bool cliUpdate(cli_t *p_cli, uint8_t rx_data);
static void cliLineClean(cli_t *p_cli);
static void cliLineAdd(cli_t *p_cli);
static void cliLineChange(cli_t *p_cli, int8_t key_up);
static void cliShowPrompt(cli_t *p_cli);
static void cliToUpper(char *str);
static bool cliRunCmd(cli_t *p_cli);
static bool cliParseArgs(cli_t *p_cli);

static int32_t cliArgsGetData(uint8_t index);
static float cliArgsGetFloat(uint8_t index);
static char* cliArgsGetStr(uint8_t index);
static bool cliArgsIsStr(uint8_t index, char *p_str);

void cliShowList(cli_args_t *args);
void cli_drv_sqc(cli_args_t *args);
void cli_sentype_sqc(cli_args_t *args);
void cli_speed_low_sqc(cli_args_t *args);
void cli_speed_high_sqc(cli_args_t *args);
void cli_remote_sqc(cli_args_t *args);
void cli_info_sqc(cli_args_t *args);
void cli_eep_save_sqc(cli_args_t *args);
void cli_speed_offset_sqc(cli_args_t *args);
void cli_battery_sqc(cli_args_t *args);
void cli_valve_sqc(cli_args_t *args);
void cli_rtk_sqc(cli_args_t *args);
void cli_str_test_sqc(cli_args_t *args);






bool cliInit(void) {
	cli_node.is_open = false;
	cli_node.is_log = false;
	cli_node.state = CLI_RX_IDLE;

	cli_node.hist_line_i = 0;
	cli_node.hist_line_last = 0;
	cli_node.hist_line_count = 0;
	cli_node.hist_line_new = false;

	cli_node.cmd_args.getData = cliArgsGetData;
	cli_node.cmd_args.getFloat = cliArgsGetFloat;
	cli_node.cmd_args.getStr = cliArgsGetStr;
	cli_node.cmd_args.isStr = cliArgsIsStr;

	cliLineClean(&cli_node);
// 조향모터 , 주행모터 , 저속, 중속, type 
	cliAdd("help", cliShowList);
	cliAdd("drv", cli_drv_sqc); // 주행 curtis / dmcs
	cliAdd("sentype", cli_sentype_sqc); // 센서타입 
	cliAdd("speed", cli_speed_offset_sqc); // idle speed offset
	cliAdd("low", cli_speed_low_sqc); // 자율주행 저속
	cliAdd("high", cli_speed_high_sqc); // 자율주행 고속
	cliAdd("remote", cli_remote_sqc); // 리모컨 종류 jeico / rfmaster	
	cliAdd("bat", cli_battery_sqc); // 배터리타입 
	cliAdd("valve", cli_valve_sqc); // 후적밸브
	cliAdd("rtk", cli_rtk_sqc); // 후적밸브
	cliAdd("info", cli_info_sqc); // 설정값 정보
	cliAdd("save", cli_eep_save_sqc); // 설정값 저장
	return true;
}

bool cliOpen(uint8_t ch, uint32_t baud) {
	cli_node.ch = ch;
	cli_node.baud = baud;

	cli_node.is_open = uartOpen(ch, baud);

	return cli_node.is_open;
}

bool cliOpenLog(uint8_t ch, uint32_t baud) {
	bool ret;

	cli_node.log_ch = ch;
	cli_node.log_baud = baud;

	ret = uartOpen(ch, baud);

	if (ret == true) {
		cli_node.is_log = true;
	}
	return ret;
}

bool cliLogClose(void) {
	cli_node.is_log = false;
	return true;
}

void cliShowLog(cli_t *p_cli) {
	if (cli_node.is_log == true) {
		uartPrintf(p_cli->log_ch, "Cursor  : %d\n", p_cli->line.cursor);
		uartPrintf(p_cli->log_ch, "Count   : %d\n", p_cli->line.count);
		uartPrintf(p_cli->log_ch, "buf_len : %d\n", p_cli->line.buf_len);
		uartPrintf(p_cli->log_ch, "buf     : %s\n", p_cli->line.buf);
		uartPrintf(p_cli->log_ch, "line_i  : %d\n", p_cli->hist_line_i);
		uartPrintf(p_cli->log_ch, "line_lt : %d\n", p_cli->hist_line_last);
		uartPrintf(p_cli->log_ch, "line_c  : %d\n", p_cli->hist_line_count);

		for (int i = 0; i < p_cli->hist_line_count; i++) {
			uartPrintf(p_cli->log_ch, "buf %d   : %s\n", i,
					p_cli->line_buf[i].buf);
		}
		uartPrintf(p_cli->log_ch, "\n");
	}
}

void cliShowPrompt(cli_t *p_cli) {
	uartPrintf(p_cli->ch, "\n\r");
	uartPrintf(p_cli->ch, CLI_PROMPT_STR);
}

bool cliMain(void) {
	if (cli_node.is_open != true) {
		return false;
	}

	if (uartAvailable(cli_node.ch) > 0) {
		cliUpdate(&cli_node, uartRead(cli_node.ch));
	}

	return true;
}

uint32_t cliAvailable(void) {
	return uartAvailable(cli_node.ch);
}

uint8_t cliRead(void) {
	return uartRead(cli_node.ch);
}

uint32_t cliWrite(uint8_t *p_data, uint32_t length) {
	return uartWrite(cli_node.ch, p_data, length);
}

bool cliUpdate(cli_t *p_cli, uint8_t rx_data) {
	bool ret = false;
	uint8_t tx_buf[8];
	cli_line_t *line;

	line = &p_cli->line;

	if (p_cli->state == CLI_RX_IDLE) {
		switch (rx_data) {
		// 엔터
		//
		case CLI_KEY_ENTER:
			if (line->count > 0) {
				cliLineAdd(p_cli);
				cliRunCmd(p_cli);
			}

			line->count = 0;
			line->cursor = 0;
			line->buf[0] = 0;
			cliShowPrompt(p_cli);
			break;

		case CLI_KEY_ESC:
			p_cli->state = CLI_RX_SP1;
			break;

			// DEL
			//
		case CLI_KEY_DEL:
			if (line->cursor < line->count) {
				uint8_t mov_len;

				mov_len = line->count - line->cursor;
				for (int i = 1; i < mov_len; i++) {
					line->buf[line->cursor + i - 1] =
							line->buf[line->cursor + i];
				}

				line->count--;
				line->buf[line->count] = 0;

				uartPrintf(p_cli->ch, "\x1B[1P");
			}
			break;

			// 백스페이스
			//
		case CLI_KEY_BACK:
			if (line->count > 0 && line->cursor > 0) {
				if (line->cursor == line->count) {
					line->count--;
					line->buf[line->count] = 0;
				}

				if (line->cursor < line->count) {
					uint8_t mov_len;

					mov_len = line->count - line->cursor;

					for (int i = 0; i < mov_len; i++) {
						line->buf[line->cursor + i - 1] = line->buf[line->cursor
								+ i];
					}

					line->count--;
					line->buf[line->count] = 0;
				}
			}

			if (line->cursor > 0) {
				line->cursor--;
				uartPrintf(p_cli->ch, "\b \b\x1B[1P");
			}
			break;

		default:
			if ((line->count + 1) < line->buf_len) {
				if (line->cursor == line->count) {
					uartWrite(p_cli->ch, &rx_data, 1);

					line->buf[line->cursor] = rx_data;
					line->count++;
					line->cursor++;
					line->buf[line->count] = 0;
				}
				if (line->cursor < line->count) {
					uint8_t mov_len;

					mov_len = line->count - line->cursor;
					for (int i = 0; i < mov_len; i++) {
						line->buf[line->count - i] = line->buf[line->count - i
								- 1];
					}
					line->buf[line->cursor] = rx_data;
					line->count++;
					line->cursor++;
					line->buf[line->count] = 0;

					uartPrintf(p_cli->ch, "\x1B[4h%c\x1B[4l", rx_data);
				}
			}
			break;
		}
	}

	switch (p_cli->state) {
	case CLI_RX_SP1:
		p_cli->state = CLI_RX_SP2;
		break;

	case CLI_RX_SP2:
		p_cli->state = CLI_RX_SP3;
		break;

	case CLI_RX_SP3:
		p_cli->state = CLI_RX_IDLE;

		if (rx_data == CLI_KEY_LEFT) {
			if (line->cursor > 0) {
				line->cursor--;
				tx_buf[0] = 0x1B;
				tx_buf[1] = 0x5B;
				tx_buf[2] = rx_data;
				uartWrite(p_cli->ch, tx_buf, 3);
			}
		}

		if (rx_data == CLI_KEY_RIGHT) {
			if (line->cursor < line->count) {
				line->cursor++;

				tx_buf[0] = 0x1B;
				tx_buf[1] = 0x5B;
				tx_buf[2] = rx_data;
				uartWrite(p_cli->ch, tx_buf, 3);
			}
		}

		if (rx_data == CLI_KEY_UP) {
			cliLineChange(p_cli, true);
			uartPrintf(p_cli->ch, (char*) p_cli->line.buf);
		}

		if (rx_data == CLI_KEY_DOWN) {
			cliLineChange(p_cli, false);
			uartPrintf(p_cli->ch, (char*) p_cli->line.buf);
		}

		if (rx_data == CLI_KEY_HOME) {
			uartPrintf(p_cli->ch, "\x1B[%dD", line->cursor);
			line->cursor = 0;

			p_cli->state = CLI_RX_SP4;
		}

		if (rx_data == CLI_KEY_END) {
			uint16_t mov_len;

			if (line->cursor < line->count) {
				mov_len = line->count - line->cursor;
				uartPrintf(p_cli->ch, "\x1B[%dC", mov_len);
			}
			if (line->cursor > line->count) {
				mov_len = line->cursor - line->count;
				uartPrintf(p_cli->ch, "\x1B[%dD", mov_len);
			}
			line->cursor = line->count;
			p_cli->state = CLI_RX_SP4;
		}
		break;

	case CLI_RX_SP4:
		p_cli->state = CLI_RX_IDLE;
		break;
	}

	cliShowLog(p_cli);

	return ret;
}

void cliLineClean(cli_t *p_cli) {
	p_cli->line.count = 0;
	p_cli->line.cursor = 0;
	p_cli->line.buf_len = CLI_LINE_BUF_MAX - 1;
	p_cli->line.buf[0] = 0;
}

void cliLineAdd(cli_t *p_cli) {

	p_cli->line_buf[p_cli->hist_line_last] = p_cli->line;

	if (p_cli->hist_line_count < CLI_LINE_HIS_MAX) {
		p_cli->hist_line_count++;
	}

	p_cli->hist_line_i = p_cli->hist_line_last;
	p_cli->hist_line_last = (p_cli->hist_line_last + 1) % CLI_LINE_HIS_MAX;
	p_cli->hist_line_new = true;
}

void cliLineChange(cli_t *p_cli, int8_t key_up) {
	uint8_t change_i;

	if (p_cli->hist_line_count == 0) {
		return;
	}

	if (p_cli->line.cursor > 0) {
		uartPrintf(p_cli->ch, "\x1B[%dD", p_cli->line.cursor);
	}
	if (p_cli->line.count > 0) {
		uartPrintf(p_cli->ch, "\x1B[%dP", p_cli->line.count);
	}

	if (key_up == true) {
		if (p_cli->hist_line_new == true) {
			p_cli->hist_line_i = p_cli->hist_line_last;
		}
		p_cli->hist_line_i = (p_cli->hist_line_i + p_cli->hist_line_count - 1)
				% p_cli->hist_line_count;
		change_i = p_cli->hist_line_i;
	} else {
		p_cli->hist_line_i = (p_cli->hist_line_i + 1) % p_cli->hist_line_count;
		change_i = p_cli->hist_line_i;
	}

	p_cli->line = p_cli->line_buf[change_i];
	p_cli->line.cursor = p_cli->line.count;

	p_cli->hist_line_new = false;
}

bool cliRunCmd(cli_t *p_cli) {
	bool ret = false;

	if (cliParseArgs(p_cli) == true) {
		cliPrintf("\r\n");

		cliToUpper(p_cli->argv[0]);

		for (int i = 0; i < p_cli->cmd_count; i++) {
			if (strcmp(p_cli->argv[0], p_cli->cmd_list[i].cmd_str) == 0) {
				p_cli->cmd_args.argc = p_cli->argc - 1;
				p_cli->cmd_args.argv = &p_cli->argv[1];
				p_cli->cmd_list[i].cmd_func(&p_cli->cmd_args);
				break;
			}
		}
	}

	return ret;
}

bool cliParseArgs(cli_t *p_cli) {
	bool ret = false;
	char *tok;
	char *next_ptr;
	uint16_t argc = 0;
	static const char *delim = " \f\n\r\t\v";
	char *cmdline;
	char **argv;

	p_cli->argc = 0;

	cmdline = (char*) p_cli->line.buf;
	argv = p_cli->argv;

	argv[argc] = NULL;

	for (tok = strtok_r(cmdline, delim, &next_ptr); tok;
			tok = strtok_r(NULL, delim, &next_ptr)) {
		argv[argc++] = tok;
	}

	p_cli->argc = argc;

	if (argc > 0) {
		ret = true;
	}

	return ret;
}

void cliPrintf(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	int32_t len;
	cli_t *p_cli = &cli_node;

	len = vsnprintf(p_cli->print_buffer, 256, fmt, arg);
	va_end(arg);

	uartWrite(p_cli->ch, (uint8_t*) p_cli->print_buffer, len);
}

void cliToUpper(char *str) {
	uint16_t i;
	uint8_t str_ch;

	for (i = 0; i < CLI_CMD_NAME_MAX; i++) {
		str_ch = str[i];

		if (str_ch == 0) {
			break;
		}

		if ((str_ch >= 'a') && (str_ch <= 'z')) {
			str_ch = str_ch - 'a' + 'A';
		}
		str[i] = str_ch;
	}

	if (i == CLI_CMD_NAME_MAX) {
		str[i - 1] = 0;
	}
}

int32_t cliArgsGetData(uint8_t index) {
	int32_t ret = 0;
	cli_t *p_cli = &cli_node;

	if (index >= p_cli->cmd_args.argc) {
		return 0;
	}

	ret = (int32_t) strtoul((const char*) p_cli->cmd_args.argv[index],
			(char**) NULL, (int) 0);

	return ret;
}

float cliArgsGetFloat(uint8_t index) {
	float ret = 0.0;
	cli_t *p_cli = &cli_node;

	if (index >= p_cli->cmd_args.argc) {
		return 0;
	}

	ret = (float) strtof((const char*) p_cli->cmd_args.argv[index],
			(char**) NULL);

	return ret;
}

char* cliArgsGetStr(uint8_t index) {
	char *ret = NULL;
	cli_t *p_cli = &cli_node;

	if (index >= p_cli->cmd_args.argc) {
		return 0;
	}

	ret = p_cli->cmd_args.argv[index];

	return ret;
}

bool cliArgsIsStr(uint8_t index, char *p_str) {
	bool ret = false;
	cli_t *p_cli = &cli_node;

	if (index >= p_cli->cmd_args.argc) {
		return 0;
	}

	if (strcmp(p_str, p_cli->cmd_args.argv[index]) == 0) {
		ret = true;
	}

	return ret;
}

bool cliKeepLoop(void) {
	cli_t *p_cli = &cli_node;

	if (uartAvailable(p_cli->ch) == 0) {
		return true;
	} else {
		return false;
	}
}

bool cliAdd(const char *cmd_str, void (*p_func)(cli_args_t*)) {
	bool ret = true;
	cli_t *p_cli = &cli_node;
	uint16_t index;

	if (p_cli->cmd_count >= CLI_CMD_LIST_MAX) {
		return false;
	}

	index = p_cli->cmd_count;

	strcpy(p_cli->cmd_list[index].cmd_str, cmd_str);
	p_cli->cmd_list[index].cmd_func = p_func;

	cliToUpper(p_cli->cmd_list[index].cmd_str);

	p_cli->cmd_count++;

	return ret;
}

void cliShowList(cli_args_t *args) 
{
	cli_t *p_cli = &cli_node;

	cliPrintf("\r\n");
	cliPrintf("%s========= CMD LIST =========\r\n",C_YLLW);
	cliPrintf("%s",C_NRML);

	for (int i = 0; i < p_cli->cmd_count; i++) 
	{
		cliPrintf("            ");
		cliPrintf(p_cli->cmd_list[i].cmd_str);
		cliPrintf("\r\n");
	}

	cliPrintf("%s============================\r\n",C_YLLW);
	cliPrintf("%s",C_NRML);
}


void cli_drv_sqc(cli_args_t *args) 
{
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== DRV MOTOR TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  CURTIS TPYE \r\n");
		cliPrintf("      >> NUMBER  2  DMCS   TPYE \r\n");
		cliPrintf("      EX) drv 1  -> CURTIS TYPE SET \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}


	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	if (addr == 1) // ch
	{
		load_value.Motor_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET CURTIS TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{
		load_value.Motor_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET DMCS TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}

}


void cli_sentype_sqc(cli_args_t *args)  // NPN PNP
{
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== STR SENSOR TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  NPN   TPYE \r\n");
		cliPrintf("      >> NUMBER  2  PNP   TPYE \r\n");
		cliPrintf("      EX) type 1  -> NPN TYPE SET \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}


	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	if (addr == 1) // ch
	{

		load_value.Sensor_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET NPN TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{
		load_value.Sensor_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET PNP TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}
}

void cli_speed_offset_sqc(cli_args_t *args) 
{
	uint8_t temp = 10;
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== AUTO SPEED TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("%s      >> CURRENT SPEED OFFSET = %d \r\n",C_AQUA,temp);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose 1 - 20 RANGE \r\n");
		cliPrintf("      EX) speed 10  -> SPEED OFFSET 10 SET \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}


	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	if (addr>=1 && addr<=20) 
	{
		load_value.IDLE_SPD = addr;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========= SET SPD OFFSET = %d ==========\r\n",C_YLLW,load_value.IDLE_SPD);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}
}


void cli_speed_low_sqc(cli_args_t *args) 
{
	uint8_t temp = 100;
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== AUTO SPEED TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("%s      >> CURRENT L SPEED = %d \r\n",C_AQUA,temp);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose 90 - 150 RANGE \r\n");
		cliPrintf("      EX) low 120  -> low SPEED 120 SET \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}

	
	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	if (addr>=90 && addr<=150) 
	{
		load_value.ATD_LOW_SPD = addr;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========= SET L SPD OFFSET = %d ==========\r\n",C_YLLW,load_value.ATD_LOW_SPD);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}

}


void cli_speed_high_sqc(cli_args_t *args) 
{
	uint8_t temp = 120;
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== AUTO SPEED TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("%s      >> CURRENT H SPEED = %d \r\n",C_AQUA,temp);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose 90 - 150 RANGE \r\n");
		cliPrintf("      EX) high 140  -> high SPEED 140 SET \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}


	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);

	if (addr>=90 && addr<=150) 
	{
		load_value.ATD_HIGH_SPD = addr;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========= SET H SPD OFFSET = %d ==========\r\n",C_YLLW,load_value.ATD_HIGH_SPD);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}
}


void cli_remote_sqc(cli_args_t *args) 
{
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== REMOTE TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  RF MATSER   TPYE \r\n");
		cliPrintf("      >> NUMBER  2  JEICO TPYE \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}


	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);

	if (addr == 1) // ch
	{
		load_value.Remote_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET RF_MASTER TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{
		load_value.Remote_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET JEICO TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}
}

void cli_battery_sqc(cli_args_t *args) 
{
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== BATTERY TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  ATLAS   TPYE \r\n");
		cliPrintf("      >> NUMBER  2  LONGIST  TPYE \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}
	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	

	if (addr == 1) // ch
	{
		load_value.Battery_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET ATLAS TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{	
		load_value.Battery_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET LONGIST TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}


	
}

void cli_valve_sqc(cli_args_t *args) 
{
	
	unsigned int *addr;
	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== VALVE TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  VALVE   TPYE \r\n");
		cliPrintf("      >> NUMBER  2  NONE    TPYE \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}
	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	

	if (addr == 1) // ch
	{
		load_value.Spray_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET VALVE TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{	
		load_value.Spray_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET NONE TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}


	
}


void cli_rtk_sqc(cli_args_t *args) 
{
	
	unsigned int *addr;
	

	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s======== RTK TYPE SELECT =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> YOU CAN choose TWO TYPE \r\n");
		cliPrintf("      >> NUMBER  1  RTK   TPYE \r\n");
		cliPrintf("      >> NUMBER  2  LINE  TPYE \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}
	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	

	if (addr == 1) // ch
	{
		load_value.rtk_type = 1;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET RTK TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else if ( addr == 2)
	{	
		load_value.rtk_type = 2;
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== SET LINE TYPE ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	else 
	{
		cliPrintf("%s======== CHECK NUMBER =========\r\n",C_RED);
		cliPrintf("%s======== NOT INVALID NUMBER =========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
	}


	
}

void cli_info_sqc(cli_args_t *args) 
{
	int argc = args->argc;

	if (args->argc < 1) 
	{
		cliPrintf("%s====== CHECK YOUR SETTTING INFO =======\r\n",C_YLLW);
		cliPrintf("%s",C_GREN);
		if (load_value.Motor_type == 1 )
			cliPrintf("      >> DRV MOTOR TYPE    = CURTIS \r\n");
		else
			cliPrintf("      >> DRV MOTOR TYPE    = DMCS \r\n");
		if (load_value.Sensor_type == 1 )
			cliPrintf("      >> STR SENSOR TPYE   = PNP\r\n");
		else
			cliPrintf("      >> STR SENSOR TPYE   = NPN\r\n");	
		if ( load_value.Remote_type == 1 )
			cliPrintf("      >> REMOTE TPYE       = RF_MASTER\r\n");
		else
			cliPrintf("      >> REMOTE TPYE       = JEICO\r\n");
		if ( load_value.Battery_type == 1 )
			cliPrintf("      >> BATTERY TPYE      = ATLAS\r\n");
		else 
			cliPrintf("      >> BATTERY TPYE      = LONGIST\r\n");
		if ( load_value.Spray_type == 1)
			cliPrintf("      >> VALVE TYPE        = VALVE\r\n");
		else
			cliPrintf("      >> VALVE TYPE        = NONE\r\n");
		if ( load_value.rtk_type == 1)
			cliPrintf("      >> RTK TYPE          = RTK\r\n");
		else
			cliPrintf("      >> RTK TYPE          = LINE\r\n");

			
		cliPrintf("      >> IDLE SPEED OFFSET = %d\r\n",load_value.IDLE_SPD);
		cliPrintf("      >> AUTO DRV LOW  SPD = %d\r\n",load_value.ATD_LOW_SPD);
		cliPrintf("      >> AUTO DRV HIGH SPD = %d\r\n",load_value.ATD_HIGH_SPD);
		cliPrintf("%s=======================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
		return;
	}

}


void cli_eep_save_sqc(cli_args_t *args) 
{

	int argc = args->argc;


	if (args->argc < 1) 
	{
		
		init_set_save();
		cliPrintf("%sSAVE..\r\n",C_RED);
		HAL_Delay(150);
		cliPrintf("%sSAVE....\r\n",C_RED);
		HAL_Delay(150);
		cliPrintf("%sSAVE......\r\n",C_RED);
		HAL_Delay(150);
		cliPrintf("%sCOMPLETE\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}
	
}
void cli_str_test_sqc(cli_args_t *args) 
{
	
	unsigned int *addr;

	int argc = args->argc;
	char **argv = args->argv;

	if (args->argc < 1) 
	{
		cliPrintf("%s========== STR TEST SQC ===========\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		cliPrintf("      >> NUMBER  1  TEST START  \r\n");
		cliPrintf("      >> NUMBER  2  TEST STOP  \r\n");
		cliPrintf("%s========================================\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		return;
	}
	addr = (unsigned int*) strtoul((const char*) argv[0], (char**) NULL,(int) 0);
	

	if (addr == 1) // ch
	{
		
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== TEST_START ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	}
	else 
	{	
		
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s========== TEST_STOP ===========\r\n",C_YLLW);
		cliPrintf("%s========================================\r\n",C_YLLW);
		cliPrintf("%s",C_NRML);
	} 
	

	
}
#endif 
