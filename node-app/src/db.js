const { Pool } = require('pg');

const pool = new Pool({
    user: 'kartheek',
    database: 'states'
});

module.exports = {
    async query(text, params) {
        const res = await pool.query(text, params);
        return res;
    },

    end: () => {
        pool.end();
    }
}
