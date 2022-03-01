const mongoose = require('mongoose')

const SACSchema = new mongoose.Schema({
    bois:{
        type: String,
        required: true
    },

    type:{
        type: String,
        required: true
    },

    origine:{
        type: String,
        required: true
    },

    TempsDeSechage:{
        type: Number,
        required: true
    },

    TemperatureMin:{
        type: Number,
        required: true
    }
})

module.exports = mongoose.model("UnSAC", SACSchema)