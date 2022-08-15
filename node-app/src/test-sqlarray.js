let db = require('./db');
async function f() {
    try{
        let m = await db.query('SELECT * FROM lights WHERE id = ANY($1)', [[1, 2]]);
        console.log(m);
    } catch (err) {
        console.log(err.stack);
    }
}

f();

process.on('SIGINT', function(code) {
    db.close();
})
