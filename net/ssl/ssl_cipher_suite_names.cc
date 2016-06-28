// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/ssl/ssl_cipher_suite_names.h"

#include <stdlib.h>

#include <openssl/ssl.h>

#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "net/ssl/ssl_connection_status_flags.h"

// Rather than storing the names of all the ciphersuites we eliminate the
// redundancy and break each cipher suite into a key exchange method, cipher
// and mac. For all the ciphersuites in the IANA registry, we extract each of
// those components from the name, number them and pack the result into a
// 16-bit number thus:
//   (MSB to LSB)
//   <3 bits> unused
//   <5 bits> key exchange
//   <5 bits> cipher
//   <3 bits> mac

// The following tables were generated by ssl_cipher_suite_names_generate.go,
// found in the same directory as this file.

namespace {

struct CipherSuite {
  uint16_t cipher_suite, encoded;
};

const struct CipherSuite kCipherSuites[] = {
    {0x0, 0x0},        // TLS_NULL_WITH_NULL_NULL
    {0x1, 0x101},      // TLS_RSA_WITH_NULL_MD5
    {0x2, 0x102},      // TLS_RSA_WITH_NULL_SHA
    {0x3, 0x209},      // TLS_RSA_EXPORT_WITH_RC4_40_MD5
    {0x4, 0x111},      // TLS_RSA_WITH_RC4_128_MD5
    {0x5, 0x112},      // TLS_RSA_WITH_RC4_128_SHA
    {0x6, 0x219},      // TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5
    {0x7, 0x122},      // TLS_RSA_WITH_IDEA_CBC_SHA
    {0x8, 0x22a},      // TLS_RSA_EXPORT_WITH_DES40_CBC_SHA
    {0x9, 0x132},      // TLS_RSA_WITH_DES_CBC_SHA
    {0xa, 0x13a},      // TLS_RSA_WITH_3DES_EDE_CBC_SHA
    {0xb, 0x32a},      // TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA
    {0xc, 0x432},      // TLS_DH_DSS_WITH_DES_CBC_SHA
    {0xd, 0x43a},      // TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA
    {0xe, 0x52a},      // TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA
    {0xf, 0x632},      // TLS_DH_RSA_WITH_DES_CBC_SHA
    {0x10, 0x63a},     // TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA
    {0x11, 0x72a},     // TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA
    {0x12, 0x832},     // TLS_DHE_DSS_WITH_DES_CBC_SHA
    {0x13, 0x83a},     // TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA
    {0x14, 0x92a},     // TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA
    {0x15, 0xa32},     // TLS_DHE_RSA_WITH_DES_CBC_SHA
    {0x16, 0xa3a},     // TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA
    {0x17, 0xb09},     // TLS_DH_anon_EXPORT_WITH_RC4_40_MD5
    {0x18, 0xc11},     // TLS_DH_anon_WITH_RC4_128_MD5
    {0x19, 0xb2a},     // TLS_DH_anon_EXPORT_WITH_DES40_CBC_SHA
    {0x1a, 0xc32},     // TLS_DH_anon_WITH_DES_CBC_SHA
    {0x1b, 0xc3a},     // TLS_DH_anon_WITH_3DES_EDE_CBC_SHA
    {0x2f, 0x142},     // TLS_RSA_WITH_AES_128_CBC_SHA
    {0x30, 0x442},     // TLS_DH_DSS_WITH_AES_128_CBC_SHA
    {0x31, 0x642},     // TLS_DH_RSA_WITH_AES_128_CBC_SHA
    {0x32, 0x842},     // TLS_DHE_DSS_WITH_AES_128_CBC_SHA
    {0x33, 0xa42},     // TLS_DHE_RSA_WITH_AES_128_CBC_SHA
    {0x34, 0xc42},     // TLS_DH_anon_WITH_AES_128_CBC_SHA
    {0x35, 0x14a},     // TLS_RSA_WITH_AES_256_CBC_SHA
    {0x36, 0x44a},     // TLS_DH_DSS_WITH_AES_256_CBC_SHA
    {0x37, 0x64a},     // TLS_DH_RSA_WITH_AES_256_CBC_SHA
    {0x38, 0x84a},     // TLS_DHE_DSS_WITH_AES_256_CBC_SHA
    {0x39, 0xa4a},     // TLS_DHE_RSA_WITH_AES_256_CBC_SHA
    {0x3a, 0xc4a},     // TLS_DH_anon_WITH_AES_256_CBC_SHA
    {0x3b, 0x103},     // TLS_RSA_WITH_NULL_SHA256
    {0x3c, 0x143},     // TLS_RSA_WITH_AES_128_CBC_SHA256
    {0x3d, 0x14b},     // TLS_RSA_WITH_AES_256_CBC_SHA256
    {0x3e, 0x443},     // TLS_DH_DSS_WITH_AES_128_CBC_SHA256
    {0x3f, 0x643},     // TLS_DH_RSA_WITH_AES_128_CBC_SHA256
    {0x40, 0x843},     // TLS_DHE_DSS_WITH_AES_128_CBC_SHA256
    {0x41, 0x152},     // TLS_RSA_WITH_CAMELLIA_128_CBC_SHA
    {0x42, 0x452},     // TLS_DH_DSS_WITH_CAMELLIA_128_CBC_SHA
    {0x43, 0x652},     // TLS_DH_RSA_WITH_CAMELLIA_128_CBC_SHA
    {0x44, 0x852},     // TLS_DHE_DSS_WITH_CAMELLIA_128_CBC_SHA
    {0x45, 0xa52},     // TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA
    {0x46, 0xc52},     // TLS_DH_anon_WITH_CAMELLIA_128_CBC_SHA
    {0x67, 0xa43},     // TLS_DHE_RSA_WITH_AES_128_CBC_SHA256
    {0x68, 0x44b},     // TLS_DH_DSS_WITH_AES_256_CBC_SHA256
    {0x69, 0x64b},     // TLS_DH_RSA_WITH_AES_256_CBC_SHA256
    {0x6a, 0x84b},     // TLS_DHE_DSS_WITH_AES_256_CBC_SHA256
    {0x6b, 0xa4b},     // TLS_DHE_RSA_WITH_AES_256_CBC_SHA256
    {0x6c, 0xc43},     // TLS_DH_anon_WITH_AES_128_CBC_SHA256
    {0x6d, 0xc4b},     // TLS_DH_anon_WITH_AES_256_CBC_SHA256
    {0x84, 0x15a},     // TLS_RSA_WITH_CAMELLIA_256_CBC_SHA
    {0x85, 0x45a},     // TLS_DH_DSS_WITH_CAMELLIA_256_CBC_SHA
    {0x86, 0x65a},     // TLS_DH_RSA_WITH_CAMELLIA_256_CBC_SHA
    {0x87, 0x85a},     // TLS_DHE_DSS_WITH_CAMELLIA_256_CBC_SHA
    {0x88, 0xa5a},     // TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA
    {0x89, 0xc5a},     // TLS_DH_anon_WITH_CAMELLIA_256_CBC_SHA
    {0x96, 0x162},     // TLS_RSA_WITH_SEED_CBC_SHA
    {0x97, 0x462},     // TLS_DH_DSS_WITH_SEED_CBC_SHA
    {0x98, 0x662},     // TLS_DH_RSA_WITH_SEED_CBC_SHA
    {0x99, 0x862},     // TLS_DHE_DSS_WITH_SEED_CBC_SHA
    {0x9a, 0xa62},     // TLS_DHE_RSA_WITH_SEED_CBC_SHA
    {0x9b, 0xc62},     // TLS_DH_anon_WITH_SEED_CBC_SHA
    {0x9c, 0x16f},     // TLS_RSA_WITH_AES_128_GCM_SHA256
    {0x9d, 0x177},     // TLS_RSA_WITH_AES_256_GCM_SHA384
    {0x9e, 0xa6f},     // TLS_DHE_RSA_WITH_AES_128_GCM_SHA256
    {0x9f, 0xa77},     // TLS_DHE_RSA_WITH_AES_256_GCM_SHA384
    {0xa0, 0x66f},     // TLS_DH_RSA_WITH_AES_128_GCM_SHA256
    {0xa1, 0x677},     // TLS_DH_RSA_WITH_AES_256_GCM_SHA384
    {0xa2, 0x86f},     // TLS_DHE_DSS_WITH_AES_128_GCM_SHA256
    {0xa3, 0x877},     // TLS_DHE_DSS_WITH_AES_256_GCM_SHA384
    {0xa4, 0x46f},     // TLS_DH_DSS_WITH_AES_128_GCM_SHA256
    {0xa5, 0x477},     // TLS_DH_DSS_WITH_AES_256_GCM_SHA384
    {0xa6, 0xc6f},     // TLS_DH_anon_WITH_AES_128_GCM_SHA256
    {0xa7, 0xc77},     // TLS_DH_anon_WITH_AES_256_GCM_SHA384
    {0xba, 0x153},     // TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xbb, 0x453},     // TLS_DH_DSS_WITH_CAMELLIA_128_CBC_SHA256
    {0xbc, 0x653},     // TLS_DH_RSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xbd, 0x853},     // TLS_DHE_DSS_WITH_CAMELLIA_128_CBC_SHA256
    {0xbe, 0xa53},     // TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xbf, 0xc53},     // TLS_DH_anon_WITH_CAMELLIA_128_CBC_SHA256
    {0xc0, 0x15b},     // TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256
    {0xc1, 0x45b},     // TLS_DH_DSS_WITH_CAMELLIA_256_CBC_SHA256
    {0xc2, 0x65b},     // TLS_DH_RSA_WITH_CAMELLIA_256_CBC_SHA256
    {0xc3, 0x85b},     // TLS_DHE_DSS_WITH_CAMELLIA_256_CBC_SHA256
    {0xc4, 0xa5b},     // TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256
    {0xc5, 0xc5b},     // TLS_DH_anon_WITH_CAMELLIA_256_CBC_SHA256
    {0xc001, 0xd02},   // TLS_ECDH_ECDSA_WITH_NULL_SHA
    {0xc002, 0xd12},   // TLS_ECDH_ECDSA_WITH_RC4_128_SHA
    {0xc003, 0xd3a},   // TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA
    {0xc004, 0xd42},   // TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
    {0xc005, 0xd4a},   // TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA
    {0xc006, 0xe02},   // TLS_ECDHE_ECDSA_WITH_NULL_SHA
    {0xc007, 0xe12},   // TLS_ECDHE_ECDSA_WITH_RC4_128_SHA
    {0xc008, 0xe3a},   // TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA
    {0xc009, 0xe42},   // TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
    {0xc00a, 0xe4a},   // TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA
    {0xc00b, 0xf02},   // TLS_ECDH_RSA_WITH_NULL_SHA
    {0xc00c, 0xf12},   // TLS_ECDH_RSA_WITH_RC4_128_SHA
    {0xc00d, 0xf3a},   // TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA
    {0xc00e, 0xf42},   // TLS_ECDH_RSA_WITH_AES_128_CBC_SHA
    {0xc00f, 0xf4a},   // TLS_ECDH_RSA_WITH_AES_256_CBC_SHA
    {0xc010, 0x1002},  // TLS_ECDHE_RSA_WITH_NULL_SHA
    {0xc011, 0x1012},  // TLS_ECDHE_RSA_WITH_RC4_128_SHA
    {0xc012, 0x103a},  // TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA
    {0xc013, 0x1042},  // TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA
    {0xc014, 0x104a},  // TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA
    {0xc015, 0x1102},  // TLS_ECDH_anon_WITH_NULL_SHA
    {0xc016, 0x1112},  // TLS_ECDH_anon_WITH_RC4_128_SHA
    {0xc017, 0x113a},  // TLS_ECDH_anon_WITH_3DES_EDE_CBC_SHA
    {0xc018, 0x1142},  // TLS_ECDH_anon_WITH_AES_128_CBC_SHA
    {0xc019, 0x114a},  // TLS_ECDH_anon_WITH_AES_256_CBC_SHA
    {0xc023, 0xe43},   // TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
    {0xc024, 0xe4c},   // TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
    {0xc025, 0xd43},   // TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
    {0xc026, 0xd4c},   // TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384
    {0xc027, 0x1043},  // TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
    {0xc028, 0x104c},  // TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384
    {0xc029, 0xf43},   // TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256
    {0xc02a, 0xf4c},   // TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384
    {0xc02b, 0xe6f},   // TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    {0xc02c, 0xe77},   // TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
    {0xc02d, 0xd6f},   // TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
    {0xc02e, 0xd77},   // TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384
    {0xc02f, 0x106f},  // TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
    {0xc030, 0x1077},  // TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
    {0xc031, 0xf6f},   // TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256
    {0xc032, 0xf77},   // TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384
    {0xc072, 0xe53},   // TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xc073, 0xe5c},   // TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_CBC_SHA384
    {0xc074, 0xd53},   // TLS_ECDH_ECDSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xc075, 0xd5c},   // TLS_ECDH_ECDSA_WITH_CAMELLIA_256_CBC_SHA384
    {0xc076, 0x1053},  // TLS_ECDHE_RSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xc077, 0x105c},  // TLS_ECDHE_RSA_WITH_CAMELLIA_256_CBC_SHA384
    {0xc078, 0xf53},   // TLS_ECDH_RSA_WITH_CAMELLIA_128_CBC_SHA256
    {0xc079, 0xf5c},   // TLS_ECDH_RSA_WITH_CAMELLIA_256_CBC_SHA384
    {0xc07a, 0x17f},   // TLS_RSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc07b, 0x187},   // TLS_RSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc07c, 0xa7f},   // TLS_DHE_RSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc07d, 0xa87},   // TLS_DHE_RSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc07e, 0x67f},   // TLS_DH_RSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc07f, 0x687},   // TLS_DH_RSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc080, 0x87f},   // TLS_DHE_DSS_WITH_CAMELLIA_128_GCM_SHA256
    {0xc081, 0x887},   // TLS_DHE_DSS_WITH_CAMELLIA_256_GCM_SHA384
    {0xc082, 0x47f},   // TLS_DH_DSS_WITH_CAMELLIA_128_GCM_SHA256
    {0xc083, 0x487},   // TLS_DH_DSS_WITH_CAMELLIA_256_GCM_SHA384
    {0xc084, 0xc7f},   // TLS_DH_anon_WITH_CAMELLIA_128_GCM_SHA256
    {0xc085, 0xc87},   // TLS_DH_anon_WITH_CAMELLIA_256_GCM_SHA384
    {0xc086, 0xe7f},   // TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc087, 0xe87},   // TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc088, 0xd7f},   // TLS_ECDH_ECDSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc089, 0xd87},   // TLS_ECDH_ECDSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc08a, 0x107f},  // TLS_ECDHE_RSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc08b, 0x1087},  // TLS_ECDHE_RSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xc08c, 0xf7f},   // TLS_ECDH_RSA_WITH_CAMELLIA_128_GCM_SHA256
    {0xc08d, 0xf87},   // TLS_ECDH_RSA_WITH_CAMELLIA_256_GCM_SHA384
    {0xcc13, 0x108f},  // TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305 (non-standard)
    {0xcc14, 0x0e8f},  // TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305 (non-standard)
    {0xcca8, 0x108f},  // TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
    {0xcca9, 0x0e8f},  // TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256
};

const struct {
  char name[15];
} kKeyExchangeNames[18] = {
  {"NULL"},  // 0
  {"RSA"},  // 1
  {"RSA_EXPORT"},  // 2
  {"DH_DSS_EXPORT"},  // 3
  {"DH_DSS"},  // 4
  {"DH_RSA_EXPORT"},  // 5
  {"DH_RSA"},  // 6
  {"DHE_DSS_EXPORT"},  // 7
  {"DHE_DSS"},  // 8
  {"DHE_RSA_EXPORT"},  // 9
  {"DHE_RSA"},  // 10
  {"DH_anon_EXPORT"},  // 11
  {"DH_anon"},  // 12
  {"ECDH_ECDSA"},  // 13
  {"ECDHE_ECDSA"},  // 14
  {"ECDH_RSA"},  // 15
  {"ECDHE_RSA"},  // 16
  {"ECDH_anon"},  // 17
};

const struct {
  char name[18];
} kCipherNames[18] = {
  {"NULL"},  // 0
  {"RC4_40"},  // 1
  {"RC4_128"},  // 2
  {"RC2_CBC_40"},  // 3
  {"IDEA_CBC"},  // 4
  {"DES40_CBC"},  // 5
  {"DES_CBC"},  // 6
  {"3DES_EDE_CBC"},  // 7
  {"AES_128_CBC"},  // 8
  {"AES_256_CBC"},  // 9
  {"CAMELLIA_128_CBC"},  // 10
  {"CAMELLIA_256_CBC"},  // 11
  {"SEED_CBC"},  // 12
  {"AES_128_GCM"},  // 13
  {"AES_256_GCM"},  // 14
  {"CAMELLIA_128_GCM"},  // 15
  {"CAMELLIA_256_GCM"},  // 16
  {"CHACHA20_POLY1305"},  // 17
};

const struct {
  char name[12];
} kMacNames[5] = {
  {"NULL"},  // 0
  {"HMAC-MD5"},  // 1
  {"HMAC-SHA1"},  // 2
  {"HMAC-SHA256"},  // 3
  {"HMAC-SHA384"},  // 4
  // 7 is reserved to indicate an AEAD cipher suite.
};

const int kAEADMACValue = 7;

int CipherSuiteCmp(const void* ia, const void* ib) {
  const CipherSuite* a = static_cast<const CipherSuite*>(ia);
  const CipherSuite* b = static_cast<const CipherSuite*>(ib);

  if (a->cipher_suite < b->cipher_suite) {
    return -1;
  } else if (a->cipher_suite == b->cipher_suite) {
    return 0;
  } else {
    return 1;
  }
}

bool GetCipherProperties(uint16_t cipher_suite,
                         int* out_key_exchange,
                         int* out_cipher,
                         int* out_mac) {
  CipherSuite desired = {0};
  desired.cipher_suite = cipher_suite;
  void* r = bsearch(&desired, kCipherSuites, arraysize(kCipherSuites),
                    sizeof(kCipherSuites[0]), CipherSuiteCmp);

  if (!r)
    return false;

  const CipherSuite* cs = static_cast<const CipherSuite*>(r);
  *out_key_exchange = cs->encoded >> 8;
  *out_cipher = (cs->encoded >> 3) & 0x1f;
  *out_mac = cs->encoded & 0x7;
  return true;
}

}  // namespace

namespace net {

void SSLCipherSuiteToStrings(const char** key_exchange_str,
                             const char** cipher_str,
                             const char** mac_str,
                             bool* is_aead,
                             uint16_t cipher_suite) {
  *key_exchange_str = *cipher_str = *mac_str = "???";
  *is_aead = false;

  int key_exchange, cipher, mac;
  if (!GetCipherProperties(cipher_suite, &key_exchange, &cipher, &mac))
    return;

  *key_exchange_str = kKeyExchangeNames[key_exchange].name;
  *cipher_str = kCipherNames[cipher].name;
  if (mac == kAEADMACValue) {
    *is_aead = true;
    *mac_str = NULL;
  } else {
    *mac_str = kMacNames[mac].name;
  }
}

void SSLVersionToString(const char** name, int ssl_version) {
  switch (ssl_version) {
    case SSL_CONNECTION_VERSION_SSL2:
      *name = "SSL 2.0";
      break;
    case SSL_CONNECTION_VERSION_SSL3:
      *name = "SSL 3.0";
      break;
    case SSL_CONNECTION_VERSION_TLS1:
      *name = "TLS 1.0";
      break;
    case SSL_CONNECTION_VERSION_TLS1_1:
      *name = "TLS 1.1";
      break;
    case SSL_CONNECTION_VERSION_TLS1_2:
      *name = "TLS 1.2";
      break;
    case SSL_CONNECTION_VERSION_QUIC:
      *name = "QUIC";
      break;
    default:
      NOTREACHED() << ssl_version;
      *name = "???";
      break;
  }
}

bool ParseSSLCipherString(const std::string& cipher_string,
                          uint16_t* cipher_suite) {
  int value = 0;
  if (cipher_string.size() == 6 &&
      base::StartsWith(cipher_string, "0x",
                       base::CompareCase::INSENSITIVE_ASCII) &&
      base::HexStringToInt(cipher_string, &value)) {
    *cipher_suite = static_cast<uint16_t>(value);
    return true;
  }
  return false;
}

bool IsSecureTLSCipherSuite(uint16_t cipher_suite) {
  int key_exchange, cipher, mac;
  if (!GetCipherProperties(cipher_suite, &key_exchange, &cipher, &mac))
    return false;

  // Only allow ECDHE key exchanges.
  switch (key_exchange) {
    case 14:  // ECDHE_ECDSA
    case 16:  // ECDHE_RSA
      break;
    default:
      return false;
  }

  switch (cipher) {
    case 13:  // AES_128_GCM
    case 14:  // AES_256_GCM
    case 17:  // CHACHA20_POLY1305
      break;
    default:
      return false;
  }

  // Only AEADs allowed.
  if (mac != kAEADMACValue)
    return false;

  return true;
}

bool IsTLSCipherSuiteAllowedByHTTP2(uint16_t cipher_suite) {
  int key_exchange, cipher, mac;
  if (!GetCipherProperties(cipher_suite, &key_exchange, &cipher, &mac))
    return false;

  // Only allow forward secure key exchanges.
  switch (key_exchange) {
    case 10:  // DHE_RSA
    case 14:  // ECDHE_ECDSA
    case 16:  // ECDHE_RSA
      break;
    default:
      return false;
  }

  switch (cipher) {
    case 13:  // AES_128_GCM
    case 14:  // AES_256_GCM
    case 17:  // CHACHA20_POLY1305
      break;
    default:
      return false;
  }

  // Only AEADs allowed.
  if (mac != kAEADMACValue)
    return false;

  return true;
}

const char* ECCurveName(uint16_t cipher_suite, int key_exchange_info) {
  int key_exchange, cipher, mac;
  if (!GetCipherProperties(cipher_suite, &key_exchange, &cipher, &mac))
    return nullptr;
  switch (key_exchange) {
    case 14:  // ECDHE_ECDSA
    case 16:  // ECDHE_RSA
      break;
    default:
      return nullptr;
  }
  return SSL_get_curve_name(key_exchange_info);
}

}  // namespace net