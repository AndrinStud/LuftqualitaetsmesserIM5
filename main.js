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
        console.log(data);
        return data;
    } catch (error) {
        console.error(error);
    }
}

async function main() {
    let data = await fetchData();
    console.log(data);

    let wert = data.data.airQuality;
    let zeit = data.data.creationDate;

    // Hier legst du die Art des Diagramms (Line Chart) fest und fütterst die beiden Datenarrays rein
    const config = {
        type: 'line',
        data:{
            labels: zeit,
            datasets: 
            [{
                label: 'Luftqualität in PPM',
                data: wert,
                backgroundColor: 'red',
                borderColor: 'red',
                borderWidth: 1
            }]
        }
    };

    // Erstellen des Charts
    const ctx = document.querySelector('#myChart').getContext('2d');
    new Chart(ctx, config);
}

main();