// ARQUIVO: Dashboard.jsx (Componente React com Chart.js)

import React, { useState, useEffect } from 'react';
import { Line } from 'react-chartjs-2';
import axios from 'axios';

const API_BASE_URL = 'http://localhost:8080/api/v1'; // Sua API Gateway (Spring Boot)

const Dashboard = () => {
    const [telemetryData, setTelemetryData] = useState([]);
    const [currentAlert, setCurrentAlert] = useState("Nenhum Alerta Crítico");

    useEffect(() => {
        // Busca dados de série temporal
        axios.get(`${API_BASE_URL}/telemetry/latest`).then(res => {
            setTelemetryData(res.data);
        });
        
        // Busca o alerta mais recente do MySQL
        axios.get(`${API_BASE_URL}/alerts/current`).then(res => {
            if (res.data.message) {
                setCurrentAlert(res.data.message);
            }
        });
        
        // Configura a atualização a cada 5 minutos
        const interval = setInterval(() => { /* Refaz as chamadas */ }, 300000);
        return () => clearInterval(interval);
    }, []);

    // Configuração do gráfico usando Chart.js (Tabela 4)
    const chartData = {
        labels: telemetryData.map(d => new Date(d.timestamp).toLocaleTimeString()),
        datasets: [{
            label: 'Umidade do Solo (%)',
            data: telemetryData.map(d => d.umid_solo),
            borderColor: 'rgb(75, 192, 192)',
        }]
    };

    return (
        <div className="dashboard">
            <h2 className="alert-box">{currentAlert}</h2>
            <div className="chart-container">
                {/* Visualização de dados de Séries Temporais (InfluxDB) */}
                <Line data={chartData} /> 
            </div>
            {/* ...Outras visualizações e relatórios */}
        </div>
    );
};