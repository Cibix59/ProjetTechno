const mongoose = require('mongoose')

const historiqueSchema = new mongoose.Schema({
    date:{
        type: String,
        required: true
    },
    heure:{
        type: String,
        required: true
    },
    evennement:{
        type: String,
        required: true
    },
    peripherique:{
        nom:{
            type: String,
            required: true
        },
        description:{
            type: String,
            required: true
        }
    }
},{collection : "historique"})

module.exports = mongoose.model("historique", historiqueSchema);