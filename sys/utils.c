/*! *********************************************************************************
* \file utils.c
* This is a source file for the utils module.
*
* Copyright 2013-2016 Freescale Semiconductor, Inc.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "utils.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
void Store(uint64_t value, uint8_t *dest, uint32_t size, endianness end);
uint64_t Read(uint8_t *src, uint32_t size, endianness end);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Store an integer into a 2 byte array based on the selected endianness.
*
* \param[in] value
* \param[in] dest
* \param[in] end
*
* \return none
********************************************************************************** */
void Store16(uint16_t value, uint8_t *dest, endianness end)
{
    Store(value, dest, sizeof(uint16_t), end);
}

/*! *********************************************************************************
* \brief    Store an integer into a 4 byte array based on the selected endianness.
*
* \param[in] value
* \param[in] dest
* \param[in] end
*
* \return none
********************************************************************************** */
void Store32(uint32_t value, uint8_t *dest, endianness end)
{
    Store(value, dest, sizeof(uint32_t), end);
}

/*! *********************************************************************************
* \brief    Store an integer into an 8 byte array based on the selected endianness.
*
* \param[in] value
* \param[in] dest
* \param[in] end
*
* \return none
********************************************************************************** */
void Store64(uint64_t value, uint8_t *dest, endianness end)
{
    Store(value, dest, sizeof(uint64_t), end);
}

/*! *********************************************************************************
* \brief    Read a 2 byte integer into an array
*
* \param[in] src
* \param[in] end
*
* \return an integer containing the stored bytes of the array
********************************************************************************** */
uint16_t Read16(uint8_t *src, endianness end)
{
    return (uint16_t)Read(src, sizeof(uint16_t), end);
}

/*! *********************************************************************************
* \brief    Read a 4 byte integer into an array
*
* \param[in] src
* \param[in] end
*
* \return an integer containing the stored bytes of the array
********************************************************************************** */
uint32_t Read32(uint8_t *src, endianness end)
{
    return (uint32_t)Read(src, sizeof(uint32_t), end);
}

/*! *********************************************************************************
* \brief    Read an 8 byte integer into an array
*
* \param[in] src
* \param[in] end
*
* \return an integer containing the stored bytes of the array
********************************************************************************** */
uint64_t Read64(uint8_t *src, endianness end)
{
    return Read(src, sizeof(uint64_t), end);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Store an integer into a byte array
*
* \param[in] value  the integer value to store into a byte array
* \param[in] dest   pointer to a previously allocated byte array
* \param[in] size   the size of the integer
* \param[in] end    the endianness used
*
* \return none
********************************************************************************** */
void Store(uint64_t value, uint8_t *dest, uint32_t size, endianness end)
{
    uint32_t i;
    uint8_t tmp;

    if (!dest) {
        return;
    }

    for (i = 0; i < size; i++) {
        tmp = value & 0xFF;
        if (end == _LITTLE_ENDIAN) {
            dest[i] = tmp;
        } else {
            dest[size - i - 1] = tmp;
        }
        value = value >> 8;
    }
}

/*! *********************************************************************************
* \brief    Store a byte array into an integer
*
* \param[in] src    pointer to the byte array
* \param[in] size   the size of the byte array
* \param[in] end    the endianness used
*
* \return an integer containing the bytes in the specified order
********************************************************************************** */
uint64_t Read(uint8_t *src, uint32_t size, endianness end)
{
    uint32_t i;
    uint64_t tmp = 0, value = 0;

    if (!src) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        if (end == _LITTLE_ENDIAN) {
            tmp = ((uint64_t)src[i]) << (i * 8);
        } else {
            tmp = ((uint64_t)src[size - i - 1]) << (i * 8);
        }
        value = value + tmp;
    }

    return value;
}


#define MAXLEN 80

#ifdef __linux__
#   define CONFIG_FILE "/usr/share/hsdk/hsdk.conf"
#elif _WIN32
#   define CONFIG_FILE "C:\\Python27\\DLLs\\hsdk.conf"
#   define strtok_r strtok_s
#endif

static char *trim(char *s)
{
    /* Initialize start, end pointers */
    char *s1 = s, *s2 = &s[strlen (s) - 1];

    /* Trim and delimit right side */
    while ((isspace (*s2)) && (s2 >= s1)) {
        s2--;
    }

    *(s2 + 1) = '\0';

    /* Trim left side */
    while ((isspace (*s1)) && (s1 < s2)) {
        s1++;
    }

    /* Copy finished string */
    strcpy (s, s1);
    return s;
}

ConfigParams *ParseConfig(void)
{
    ConfigParams *params = (ConfigParams *)calloc(1, sizeof(ConfigParams));

    char *s, *saveptr, buff[256];
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (fp == NULL) {
        printf("WARNING: Cannot open %s => FSCI ACKs are disabled\n", CONFIG_FILE);
        return params;  // everything is zeroed out by calloc
    }

    while ((s = fgets(buff, sizeof buff, fp)) != NULL) {

        /* Skip blank lines and comments */
        if (buff[0] == '\n' || buff[0] == '#') {
            continue;
        }

        /* Parse name/value pair from line */
        char name[MAXLEN + 1], value[MAXLEN + 1];
        s = strtok_r(buff, "=", &saveptr);
        if (s == NULL) {
            continue;
        } else {
            strncpy(name, s, MAXLEN);
            name[MAXLEN] = '\0';
        }
        s = strtok_r(NULL, "=", &saveptr);
        if (s == NULL) {
            continue;
        } else {
            strncpy(value, s, MAXLEN);
            value[MAXLEN] = '\0';
        }

        /* Get rid of trailing and leading whitespace */
        trim(value);

        if (strcmp(name, "FsciTxAck") == 0) {
            params->fsciTxAck = atoi(value);
        } else if (strcmp(name, "NumberOfRetries") == 0) {
            params->numberOfRetries = atoi(value);
        } else if (strcmp(name, "TimeoutAckMs") == 0) {
            params->timeoutAckMs = atoi(value);
        } else if (strcmp(name, "FsciRxAck") == 0) {
            params->fsciRxAck = atoi(value);
        } else {
            printf("WARNING: %s/%s: Unknown name/value pair!\n", name, value);
        }
    }

    fclose(fp);

    return params;
}
