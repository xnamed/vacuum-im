#Building Translations
qm_build_all.target = build_qm

for(LANG, TRANS_LANGS) {
    TS_FILE = $${_PRO_FILE_PWD_}/$${TRANS_SOURCE_ROOT}/translations/$${LANG}/$${TRANS_SOURCE}.ts
    QM_FILE = $${TRANS_BUILD_ROOT}/src/translations/$${TRANS_TARGET}_$${LANG}.qm
    QM_DIR = $${TRANS_BUILD_ROOT}/src/translations
    equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2): QM_DIR = $$replace(QM_DIR, /, \\\\)

   LRELEASE = $$[QT_INSTALL_BINS]/lrelease
   equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2): LRELEASE = $$replace(LRELEASE, \\\\, \\\\)
   equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2): LRELEASE = $$replace(LRELEASE, /, \\\\)

   QM_COMMAND = $$QMAKE_MKDIR $${QM_DIR} && $${LRELEASE} -compress $${TS_FILE} -qm $${QM_FILE}
   equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2): QM_COMMAND = ( if not exist $${QM_DIR} ( $$QMAKE_MKDIR $${QM_DIR} ) ) & $${LRELEASE} $${TS_FILE} -qm $${QM_FILE}
   eval(qm_$${TRANS_TARGET}_build_$${LANG}.target   = $${QM_FILE})
   eval(qm_$${TRANS_TARGET}_build_$${LANG}.depends  = $${TS_FILE})
   eval(qm_$${TRANS_TARGET}_build_$${LANG}.commands = $${QM_COMMAND})

  qm_build_all.depends += qm_$${TRANS_TARGET}_build_$${LANG}
  QMAKE_EXTRA_TARGETS  += qm_$${TRANS_TARGET}_build_$${LANG}

  win32|os2: QM_FILE = $$replace(QM_FILE, /, \\\\)
  QMAKE_DISTCLEAN += $${QM_FILE}
}

QMAKE_EXTRA_TARGETS  += qm_build_all
POST_TARGETDEPS += $${qm_build_all.target}

#Install Translations
#!android {
for(LANG, TRANS_LANGS) {
   INST_DIR  = $${INSTALL_TRANSLATIONS}
   win32|os2: INST_DIR = $$replace(INST_DIR, /, \\\\)
	
   INST_FILE = $${TRANS_BUILD_ROOT}/src/translations/$${TRANS_TARGET}_$${LANG}.qm
   win32|os2: INST_FILE = $$replace(INST_FILE, /, \\\\)
	
   eval(qm_$${TRANS_TARGET}_inst_$${LANG}.path  = $${INST_DIR})
   eval(qm_$${TRANS_TARGET}_inst_$${LANG}.files = $${INST_FILE})

   #If file is not exits right now, qmake will not install it
   !exists($${INST_FILE}): eval(qm_$${TRANS_TARGET}_inst_$${LANG}.extra = "-$(INSTALL_FILE)" \\\"$${INST_FILE}\\\" \\\"$(INSTALL_ROOT)$${INST_DIR}\\\")

  INSTALLS += qm_$${TRANS_TARGET}_inst_$${LANG}
  }
#}
