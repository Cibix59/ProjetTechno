

const mongoose = require('mongoose')

const historiqueSchema = new mongoose.Schema({
    date: {
        type: String,
        required: true
    },
    heure: {
        type: String,
        required: true
    },
    topic: {
        type: String,
        required: true
    },
    nomPeripherique: {
        type: String,
        required: true
    },
    payload: {
        type: String,
        required: true
    }

},{collection : "historique"})

module.exports = mongoose.model("historique", historiqueSchema);