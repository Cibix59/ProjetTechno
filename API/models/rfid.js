
const mongoose = require('mongoose')

const rfidSchema = new mongoose.Schema({
    codeRFID: {
        type: String,
        required: true
    },
    porte: {
        type: Array,
        required: false
    }
/*     ,
    user: {
        type: Array,
        required: false
    } */

},{collection : "rfid"})

module.exports = mongoose.model("rfid", rfidSchema);