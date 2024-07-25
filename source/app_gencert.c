/*
 * gencert.c
 *
 *  Created on: Jul 24, 2024
 *      Author: user
 */
#include <stdio.h>
#include "x509_crt.h"

/* mbedTLS includes. */
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/sha256.h"
#include "mbedtls/platform.h"
#include "mbedtls/threading.h"

#define DFL_SERIAL_NUMBER		"123456789"
#define DFL_NOT_BEFORE          "20010101000000"
#define DFL_NOT_AFTER           "20700101000000"


// reference code from https://github.com/Mbed-TLS/mbedtls/issues/7050
static int generate_ss_cert(mbedtls_pk_context *key, unsigned char* pem_buffer, size_t buf_len, const char* subject_name) {
	int ret = 1;
	mbedtls_mpi serial;
	mbedtls_x509write_cert crt;
	mbedtls_ctr_drbg_context ctr_drbg;

	mbedtls_x509write_crt_init(&crt);
	mbedtls_mpi_init(&serial);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	if ((ret = mbedtls_mpi_read_string(&serial, 10, DFL_SERIAL_NUMBER)) != 0) {
		goto exit;
	}

	mbedtls_x509write_crt_set_subject_key(&crt, key);
	mbedtls_x509write_crt_set_issuer_key(&crt, key);

	if ((ret = mbedtls_x509write_crt_set_subject_name(&crt, subject_name)) != 0) {
		goto exit;
	}
	if ((ret = mbedtls_x509write_crt_set_issuer_name(&crt, subject_name)) != 0) {
		goto exit;
	}

	mbedtls_x509write_crt_set_version(&crt, MBEDTLS_X509_CRT_VERSION_3);
	mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);

	ret = mbedtls_x509write_crt_set_serial(&crt, &serial);
	if (ret != 0) {
		goto exit;
	}

	ret = mbedtls_x509write_crt_set_validity(&crt, DFL_NOT_BEFORE, DFL_NOT_AFTER);
	if (ret != 0) {
		goto exit;
	}

	ret = mbedtls_x509write_crt_set_basic_constraints(&crt, 1, 0);
	if (ret != 0) {
		goto exit;
	}

	ret = mbedtls_x509write_crt_pem(&crt, pem_buffer, buf_len, mbedtls_ctr_drbg_random, &ctr_drbg);
	if (ret != 0) {
		return ret;
	}

exit: mbedtls_x509write_crt_free(&crt);
	mbedtls_mpi_free(&serial);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	return ret;
}

static int make_key(mbedtls_pk_context* key, unsigned char* key_buffer, size_t buf_len) {
	int ret;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;

	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	ret = mbedtls_pk_setup(key, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);

	ret = mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(*key), mbedtls_ctr_drbg_random, &ctr_drbg);
	ret = mbedtls_pk_write_pubkey_pem(key, key_buffer, buf_len);
	return ret;
}

static unsigned char cert_buffer[1500] = {0};
static unsigned char key_buffer[1000] = {0};

void gencert_test(void) {
	int ret;
	mbedtls_pk_context key;
	mbedtls_pk_init(&key);

	ret = make_key(&key, key_buffer, sizeof(key_buffer));
	if (0 != ret) {
		return;
	}
	size_t ret_len = sizeof(cert_buffer);
	ret = generate_ss_cert(&key, cert_buffer, ret_len, "CN=IoTConnectDevCert,O=Avnet,C=US");
	if (0 != ret) {
		return;
	}
	printf("%s\n", key_buffer);
	printf("%s\n", cert_buffer);

}

