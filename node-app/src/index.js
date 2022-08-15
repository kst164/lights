const http = require('http');
const axios = require('axios');
const url = require('url');
const db = require('./db');
const lt = require('localtunnel');

http.createServer(async function (req, res) {
    let q = url.parse(req.url, true);
    let path = q.pathname;
    let query = q.query;
    let returnCode = 200;
    let contentType = 'application/json';
    let out = {};

    if (path == "/get_states") {

        // Return all lights and their states

        const { rows } = await db.query('SELECT * FROM lights');
        for (i in rows) {
            let row = rows[i];
            out[row.id] = row.state;
        }
    } else if (path == "/get_state") {

        // Return state of lights provided in lightID parameter

        if (Array.isArray(query.lightID)) { // Multiple lights are requested
            for (i in query.lightID) {
                let id = query.lightID[i];
                const { rows } = await db.query("SELECT * FROM lights WHERE id = $1", [id]);
                if (rows.length) {      // If light exists
                    out[rows[0].id] = rows[0].state;
                }
            }
        } else if (query.lightID) {         // Only one light
            let id = query.lightID;
            const { rows } = await db.query("SELECT * FROM lights WHERE id = $1", [id]);
            if (rows.length) {      // If light exists
                out[rows[0].id] = rows[0].state;
            }
        }

    } else if (path == "/esp_get_states") {

        // Called when board turns on, returns states of all lights with given boardID parameter

        if (!Array.isArray(query.boardID) && query.boardID) {         // Only one boardID
            const { rows } = await db.query("SELECT * FROM lights WHERE board_id = $1", [query.boardID]);
            if (rows.length) {      // If board has lights
                for (i in rows) {
                    out[rows[i].id] = rows[i].state;
                }
            }

            db.query('UPDATE esps SET addr = $1 WHERE board_id = $2', [query.ip, query.boardID])
                .catch(err => {
                    console.error(err.stack)
                });
        }


    } else if (path == "/toggle" || path == '/set_on' || path == '/set_off') {

        // Toggle state of lights provided in lightID parameter

        if (Array.isArray(query.lightID)) { // Multiple lights are requested
            for (i in query.lightID) {

                // request to change multiple lights only comes from one device
                 const row = await handleChange(path, query.lightID[i], false);

                if (row) {
                    out[row.id] = row.state;
                }
            }
        } else if (query.lightID) {         // Only one light
            const row = await handleChange(path, query.lightID, query.sameDevice);

            if (row) {
                out[row.id] = row.state;
            }
        }
    } else if (path == '/get_req') {
        out = q;
    } else {

        // Unsupported URL

        returnCode = 400;
        contentType = 'text/plain';
        out = '400 Bad Request';
    }

    if (contentType == 'application/json') {
        outString = JSON.stringify(out);
    } else {
        outString = out;
    }
    console.log('href:      ', q.href);
    console.log('outString: ', outString);
    console.log('');

    res.writeHead(returnCode, {
        'Content-Type': contentType,
        'Content-Length' : outString.length
    });
    res.write(outString);
    res.end();
}).listen(8080);

async function handleChange(path, lightID, sameDevice) {

    // for /toggle, /set_on, /set_off

    let sql;
    if (path == '/toggle') {
        sql = 'NOT state';
    } else if (path == '/set_on') {
        sql = 'TRUE';
    } else {
        sql = 'FALSE';
    }

    if (isNaN(lightID)) {
        return;
    }

    try {
        const { rows } = await db.query("SELECT * FROM esps WHERE board_id IN (SELECT board_id FROM lights WHERE id = $1)", [lightID]);

        // If no such light
        if (!rows.length) {
            return;
        }

        const row = await db.query('UPDATE lights SET state = ' + sql + ' WHERE id = $1 RETURNING *', [lightID]);

        // sameDevice: query came from the device controlling the light
        if (sameDevice !== "1") {
            let url = new URL(row.rows[0].state ? '/set_on' : 'set_off', `http://${rows[0].addr}:${rows[0].port}/`);
            url.searchParams.set('lightID', lightID);

            axios.get(url.href)
                .catch(err => {
                    console.error(err.stack)
                });
        }

        return row.rows[0];
    } catch (err) {
        console.error(err.stack);
    }
}

let tunnel;

(async function() {
    tunnel = await lt({
        port: 8080,
        subdomain: 'kst164'
    });

    tunnel.on('error', (err) => {
        console.error(err.message);
    });

})();

function handleEnd(code) {
    console.log("\nExit code: ", code);
    db.end();
    tunnel.close();
    process.exit();
}
process.on('SIGINT', handleEnd);
process.on('SIGTERM', handleEnd);
