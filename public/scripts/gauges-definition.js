// Create Temperature Gauge
function createTemperatureGauge() {
    var gauge = new LinearGauge({
        renderTo: 'gauge-temperature',
        width: 150,
        height: 400,
        units: "Watts (W)",
        minValue: 0,
        startAngle: 90,
        ticksAngle: 180,
        maxValue: 1000,
        colorValueBoxRect: "#049faa",
        colorValueBoxRectEnd: "#049faa",
        colorValueBoxBackground: "#f1fbfc",
        valueDec: 2,
        valueInt: 2,
        majorTicks: [
            "50",
            "100",
            "150",
            "200",
            "250",
            "300",
            "400",
            "500",
            "600",
            "700",
            "800",
            "900",
            "1000",
        ],
        minorTicks: 4,
        strokeTicks: true,
        highlights: [
            {
                "from": 700,
                "to": 1000,
                "color": "rgba(200, 50, 50, .75)"
            }
        ],
        colorPlate: "#fff",
        colorBarProgress: "#CC2936",
        colorBarProgressEnd: "#049faa",
        borderShadowWidth: 0,
        borders: false,
        needleType: "arrow",
        needleWidth: 5,
        needleCircleSize: 2,
        needleCircleOuter: true,
        needleCircleInner: false,
        animationDuration: 1500,
        animationRule: "linear",
        barWidth: 10,
    });
    return gauge;
}

// Create Humidity Gauge
function createHumidityGauge(){
    var gauge = new RadialGauge({
        renderTo: 'gauge-humidity',
        width: 300,
        height: 300,
        units: "Amps (A)",
        minValue: 0,
        maxValue: 30,
        colorValueBoxRect: "#049faa",
        colorValueBoxRectEnd: "#049faa",
        colorValueBoxBackground: "#f1fbfc",
        valueInt: 2,
        majorTicks: [
            "0",
            "5",
            "10",
            "15",
            "20",
            "30"
        ],
        minorTicks: 4,
        strokeTicks: true,
        highlights: [
            {
                "from": 22,
                "to": 30,
                "color": "#F44336"
            }
        ],
        colorPlate: "#fff",
        borderShadowWidth: 0,
        borders: false,
        needleType: "line",
        colorNeedle: "#000000",
        colorNeedleEnd: "#ED1B0C",
        needleWidth: 4,
        needleCircleSize: 5,
        colorNeedleCircleOuter: "#007F80",
        needleCircleOuter: true,
        needleCircleInner: false,
        animationDuration: 1500,
        animationRule: "linear"
    });
    return gauge;
}