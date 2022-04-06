
const mongoose = require('mongoose')

const rfidSchema = new mongoose.Schema({
    utilisateur: {
        type: String,
        required: true
    },
    codeRFID: {
        type: String,
        required: true
    },
    porte: {
        type: String,
        required: false
    }

}, { collection: "rfid" })

module.exports = mongoose.model("rfid", rfidSchema);