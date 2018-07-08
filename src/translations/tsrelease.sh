#!/bin/sh

TS_DIR=.
QM_DIR=.

lrelease=lrelease
if which -s lrelease-qt5; then
	lrelease=lrelease-qt5
fi

find "${TS_DIR}" -name '*.ts' | sed "s,^${TS_DIR}/,," | \
while read ts; do
	${lrelease} ${TS_DIR}/${ts} -qm ${QM_DIR}/${ts%.ts}.qm 
done
