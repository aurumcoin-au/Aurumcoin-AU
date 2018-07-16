DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

AURUMCOIN_CONFIG_INCLUDES=-I$(builddir)/config
AURUMCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

AURUMCOIN_INCLUDES += -I$(srcdir)/secp256k1/include
AURUMCOIN_INCLUDES += -I$(srcdir)/univalue/include

LIBAURUMCOIN_SERVER=libaurumcoin_server.a
LIBAURUMCOIN_WALLET=libaurumcoin_wallet.a
LIBAURUMCOIN_COMMON=libaurumcoin_common.a
LIBAURUMCOIN_CLI=libaurumcoin_cli.a
LIBAURUMCOIN_UTIL=libaurumcoin_util.a
LIBAURUMCOIN_CRYPTO=crypto/libaurumcoin_crypto.a
LIBAURUMCOINQT=qt/libaurumcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
  
$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libaurumcoin_crypto.a \
  libaurumcoin_util.a \
  libaurumcoin_common.a \
  libaurumcoin_server.a \
  libaurumcoin_cli.a
if ENABLE_WALLET
AURUMCOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libaurumcoin_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libaurumcoin_zmq.a
endif

if BUILD_AURUMCOIN_LIBS
lib_LTLIBRARIES = libaurumcoinconsensus.la
LIBAURUMCOIN_CONSENSUS=libaurumcoinconsensus.la
else
LIBAURUMCOIN_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_AURUMCOIND
  bin_PROGRAMS += aurumcoind
endif

if BUILD_AURUMCOIN_UTILS
  bin_PROGRAMS += aurumcoin-cli aurumcoin-tx
endif

.PHONY: FORCE check-symbols check-security
# aurumcoin core #
AURUMCOIN_CORE_H = \
  addrman.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  prevector.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libaurumcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between aurumcoind and aurumcoin-qt
libaurumcoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libaurumcoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(AURUMCOIN_CORE_H)

if ENABLE_ZMQ
LIBAURUMCOIN_ZMQ=libaurumcoin_zmq.a

libaurumcoin_zmq_a_CPPFLAGS = $(AURUMCOIN_INCLUDES) $(ZMQ_CFLAGS)
libaurumcoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between aurumcoind and aurumcoin-qt, but only linked
# when wallet enabled
libaurumcoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
libaurumcoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(AURUMCOIN_CORE_H)

# crypto primitives library
crypto_libaurumcoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_CONFIG_INCLUDES)
crypto_libaurumcoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libaurumcoin_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# common: shared between aurumcoind, and aurumcoin-qt and non-server tools
libaurumcoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
libaurumcoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(AURUMCOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libaurumcoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
libaurumcoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(AURUMCOIN_CORE_H)

if GLIBC_BACK_COMPAT
libaurumcoin_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between aurumcoin-cli and aurumcoin-qt
libaurumcoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
libaurumcoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaurumcoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(AURUMCOIN_CORE_H)

nodist_libaurumcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# aurumcoind binary #
aurumcoind_SOURCES = aurumcoind.cpp
aurumcoind_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
aurumcoind_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
aurumcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
aurumcoind_SOURCES += aurumcoind-res.rc
endif

aurumcoind_LDADD = \
  $(LIBAURUMCOIN_SERVER) \
  $(LIBAURUMCOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBAURUMCOIN_UTIL) \
  $(LIBAURUMCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
aurumcoind_LDADD += $(LIBAURUMCOIN_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
aurumcoind_LDADD += libaurumcoin_wallet.a
endif

aurumcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# aurumcoin-cli binary #
aurumcoin_cli_SOURCES = aurumcoin-cli.cpp
aurumcoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES) $(EVENT_CFLAGS)
aurumcoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
aurumcoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
aurumcoin_cli_SOURCES += aurumcoin-cli-res.rc
endif

aurumcoin_cli_LDADD = \
  $(LIBAURUMCOIN_CLI) \
  $(LIBUNIVALUE) \
  $(LIBAURUMCOIN_UTIL)

aurumcoin_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# aurumcoin-tx binary #
aurumcoin_tx_SOURCES = aurumcoin-tx.cpp
aurumcoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(AURUMCOIN_INCLUDES)
aurumcoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
aurumcoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
aurumcoin_tx_SOURCES += aurumcoin-tx-res.rc
endif

aurumcoin_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBAURUMCOIN_COMMON) \
  $(LIBAURUMCOIN_UTIL) \
  $(LIBAURUMCOIN_CRYPTO) \
  $(LIBSECP256K1)

aurumcoin_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# aurumcoinconsensus library #
if BUILD_AURUMCOIN_LIBS
include_HEADERS = script/aurumcoinconsensus.h
libaurumcoinconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/aurumcoinconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libaurumcoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libaurumcoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libaurumcoinconsensus_la_LIBADD = $(LIBSECP256K1)
libaurumcoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_AURUMCOIN_INTERNAL
libaurumcoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
