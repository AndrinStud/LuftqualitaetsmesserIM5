/*****************************************************************************************************************
 *  main.js 
 *  http://www.waschbaeraugen.ch/main.js
 *  Daten als JSON-String vom Server holen (unload.php), sobald main.js dieses Script per HTTP aufruft (fetch())
******************************************************************************************************************/

async function fetchData() {
    try{
        // const response = await fetch('https://localhost/07_sensor2website_improved/db2website.php');
        // const response = await fetch('https://650665-4.web.fhgr.education/07_sensor2website_improved/db2website.php');
        const response = await fetch('http://www.waschbaeraugen.ch/php/unload.php');
        const data = await response.json();
        return data;
    } catch (error) {
        console.error(error);
    }
}

async function main() {
    let data = await fetchData();
    console.log(data);

    // Raum Loop
    /*data.data.forEach(room => {
        console.log(room);
    });*/

    let eduzone = data.data.Eduzone;
    let Foyer = data.data.Foyer;
    let i011 = data.data["I-0.11"];
    let i030 = data.data["I-0.30"];
    let i031 = data.data["I-0.31"];

    // Reverse the data arrays
    eduzone.creationDate.reverse();
    eduzone.airQuality.reverse();
    Foyer.creationDate.reverse();
    Foyer.airQuality.reverse();
    i011.creationDate.reverse();
    i011.airQuality.reverse();
    i030.creationDate.reverse();
    i030.airQuality.reverse();
    i031.creationDate.reverse();
    i031.airQuality.reverse();

    // Hier legst du die Art des Diagramms (Line Chart) fest und fütterst die beiden Datenarrays rein
    const config = {
        type: 'line',
        data:{
            labels: eduzone.creationDate,
            datasets: 
            [{
                label: 'Luftqualität Eduzone in PPM',
                data: eduzone.airQuality,
                backgroundColor: 'red',
                borderColor: 'red',
                borderWidth: 1
            },
            {
                label: 'Luftqualität Foyer in PPM',
                data: Foyer.airQuality,
                backgroundColor: 'blue',
                borderColor: 'blue',
                borderWidth: 1
            },
            {
                label: 'Luftqualität I-0.11 in PPM',
                data: i011.airQuality,
                backgroundColor: 'green',
                borderColor: 'green',
                borderWidth: 1
            },
            {
                label: 'Luftqualität I-0.30 in PPM',
                data: i030.airQuality,
                backgroundColor: 'purple',
                borderColor: 'purple',
                borderWidth: 1
            },
            {
                label: 'Luftqualität I-0.31 in PPM',
                data: i031.airQuality,
                backgroundColor: 'orange',
                borderColor: 'orange',
                borderWidth: 1
            }]
        }
    };

    // Erstellen des Charts
    const ctx = document.querySelector('#myChart').getContext('2d');
    new Chart(ctx, config);
}

main();