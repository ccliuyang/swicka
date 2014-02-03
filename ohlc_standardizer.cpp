#include "ohlc_standardizer.h"

OHLCStandardizer::OHLCStandardizer(OHLCProvider* source) {
	this->source = source;
	sourceEmpty = true;
	for (QDateTime start = source->getMinimum(); start < source->getMaximum(); start = start.addSecs(source->getQuantumSeconds())) {
		OHLC tick;

		if (!source->tryGetData(start, tick)) continue;
		if (sourceEmpty) {
			sourceClosure = tick;
			sourceEmpty = false;
		} else {
			sourceClosure << tick;
		}
	}
}

QDateTime OHLCStandardizer::getMinimum() { return source->getMinimum(); }
QDateTime OHLCStandardizer::getMaximum() { return source->getMaximum(); }
int OHLCStandardizer::getQuantumSeconds() { return source->getQuantumSeconds(); }
bool OHLCStandardizer::tryGetData(QDateTime start, OHLC& output) {
	if (source->tryGetData(start, output)) {
		output.standardizeTo(sourceClosure);
		return true;
	} else {
		return false;
	}
}
