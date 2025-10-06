# ARQUIVO: ia_service.py (Pode ser um microsserviço Flask/FastAPI ou um script chamado pelo Java)

import tensorflow as tf
# ... outras dependências (OpenCV/Numpy - Tabela 4)

# 1. Carrega o modelo CNN otimizado (Tabela 5)
interpreter = tf.lite.Interpreter(model_path="model_v2.tflite")
interpreter.allocate_tensors()
# ...

def classificar_imagem(image_path):
    """Executa a inferência de imagem na borda ou na nuvem."""
    # ... Lógica de pré-processamento (224x224 pixels - Tabela 5)
    
    # SIMULAÇÃO: 
    return "Saudável" if random.random() > 0.3 else "Sinais de falta de água"

def run_predictive_logic(temp_ar, umid_solo):
    """Implementa as regras condicionais (Figuras 5 e 6)"""
    alerta_risco = None
    
    # REGRA: Déficit Hídrico (Figura 5)
    if umid_solo < 30 and temp_ar > 28:
        alerta_risco = "Probabilidade_de_Deficit_Hidrico = ALTA"

    # REGRA: Risco Fúngico (Figura 6)
    elif umid_solo > 60 and 20 <= temp_ar <= 25:
        alerta_risco = "Risco_Fungico = ALTO"
        
    return alerta_risco

def cruzar_informacoes(dados_sensores, image_path):
    """Implementa o cruzamento de informações (Tabela 8)."""
    risco_sensor = run_predictive_logic(dados_sensores["temp_ar"], dados_sensores["umid_solo"])
    status_ia = classificar_imagem(image_path)
    
    # 1º CENÁRIO: CONFIRMAR Alerta para Irrigação
    if status_ia == "Sinais de falta de água" and "Deficit_Hidrico = ALTA" in risco_sensor:
        return "ALERTA: Suas plantas precisam de água AGORA!"
        
    # 2º CENÁRIO: PREDITAR Alerta de Doença
    if status_ia == "Saudável" and "Risco_Fungico = ALTO" in risco_sensor:
        return "ALERTA PREVENTIVO: Condições ideais para fungos. Monitore!"
        
    return None