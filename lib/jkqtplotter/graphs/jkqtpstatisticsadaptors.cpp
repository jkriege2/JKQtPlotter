/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsbase.h"


JKQTPXFunctionLineGraph *jkqtpstatAddLinearRegression(JKQTPXYGraph *datagraph, double *coeffA, double *coeffB, bool fixA, bool fixB) {
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddLinearRegression(plt, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), coeffA, coeffB, fixA, fixB);
}

JKQTPXFunctionLineGraph *jkqtpstatAddLinearWeightedRegression(JKQTPXYGraph *datagraph, double *coeffA, double *coeffB, bool fixA, bool fixB)
{
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    JKQTPYGraphErrorData* ge=dynamic_cast<JKQTPYGraphErrorData*>(datagraph);

    JKQTPASSERT_M(ge!=nullptr, "datagraph needs to be convertible to JKQTPYGraphErrorData with a dynamic_cast!");
    return jkqtpstatAddLinearWeightedRegression(plt, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), ds->begin(ge->getYErrorColumn()), ds->end(ge->getYErrorColumn()), coeffA, coeffB, fixA, fixB, &jkqtp_inversePropSaveDefault<double>);
}

JKQTPXFunctionLineGraph *jkqtpstatAddRobustIRLSLinearRegression(JKQTPXYGraph *datagraph, double *coeffA, double *coeffB, bool fixA, bool fixB, double p, int iterations)
{
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddRobustIRLSLinearRegression(plt, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), coeffA, coeffB, fixA, fixB, p, iterations);
}


JKQTPXFunctionLineGraph *jkqtpstatAddRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double *coeffA, double *coeffB, bool fixA, bool fixB) {
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddRegression(plt, type, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), coeffA, coeffB, fixA, fixB);
}

JKQTPXFunctionLineGraph *jkqtpstatAddWeightedRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double *coeffA, double *coeffB, bool fixA, bool fixB)
{
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    JKQTPYGraphErrorData* ge=dynamic_cast<JKQTPYGraphErrorData*>(datagraph);

    JKQTPASSERT_M(ge!=nullptr, "datagraph needs to be convertible to JKQTPYGraphErrorData with a dynamic_cast!");
    return jkqtpstatAddWeightedRegression(plt, type, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), ds->begin(ge->getYErrorColumn()), ds->end(ge->getYErrorColumn()), coeffA, coeffB, fixA, fixB, &jkqtp_inversePropSaveDefault<double>);
}

JKQTPXFunctionLineGraph *jkqtpstatAddRobustIRLSRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double *coeffA, double *coeffB, bool fixA, bool fixB, double p, int iterations)
{
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddRobustIRLSRegression(plt, type, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()), coeffA, coeffB, fixA, fixB, p, iterations);
}

JKQTPXFunctionLineGraph *jkqtpstatAddPolyFit(JKQTPXYGraph *datagraph, size_t P) {
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddPolyFit(plt, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()),P);
}
