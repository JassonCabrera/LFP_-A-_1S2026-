#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>

using namespace std;

// ===================== ESTRUCTURAS =====================

struct Estudiante {
    int carnet;
    string nombre;
    string apellido;
    string carrera;
    int semestre;
};

struct Curso {
    int codigo;
    string nombre;
    int creditos;
    int semestre;
    string carrera;
};

struct Nota {
    int carnet;
    int codigoCurso;
    double nota;
    string ciclo;
    int anio;
};

// ===================== VECTORES GLOBALES =====================

vector<Estudiante> listaEstudiantes;
vector<Curso> listaCursos;
vector<Nota> listaNotas;

// ===================== FUNCION LIMPIAR SALTO WINDOWS =====================

void limpiarLinea(string &linea) {
    linea.erase(remove(linea.begin(), linea.end(), '\r'), linea.end());
}

// ===================== CARGAR ESTUDIANTES =====================

void cargarEstudiantes() {

    ifstream archivo("estudiantes.lfp");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir estudiantes.lfp" << endl;
        return;
    }

    listaEstudiantes.clear();

    string linea;

    while (getline(archivo, linea)) {

        limpiarLinea(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        string carnet, nombre, apellido, carrera, semestre;

        getline(ss, carnet, ',');
        getline(ss, nombre, ',');
        getline(ss, apellido, ',');
        getline(ss, carrera, ',');
        getline(ss, semestre, ',');

        Estudiante e;
        e.carnet = stoi(carnet);
        e.nombre = nombre;
        e.apellido = apellido;
        e.carrera = carrera;
        e.semestre = stoi(semestre);

        listaEstudiantes.push_back(e);
    }

    archivo.close();

    cout << "Estudiantes cargados: " << listaEstudiantes.size() << endl;
}

// ===================== CARGAR CURSOS =====================

void cargarCursos() {

    ifstream archivo("cursos.lfp");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir cursos.lfp" << endl;
        return;
    }

    listaCursos.clear();

    string linea;

    while (getline(archivo, linea)) {

        limpiarLinea(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        string codigo, nombre, creditos, semestre, carrera;

        getline(ss, codigo, ',');
        getline(ss, nombre, ',');
        getline(ss, creditos, ',');
        getline(ss, semestre, ',');
        getline(ss, carrera, ',');

        Curso c;
        c.codigo = stoi(codigo);
        c.nombre = nombre;
        c.creditos = stoi(creditos);
        c.semestre = stoi(semestre);
        c.carrera = carrera;

        listaCursos.push_back(c);
    }

    archivo.close();

    cout << "Cursos cargados: " << listaCursos.size() << endl;
}

// ===================== CARGAR NOTAS =====================

void cargarNotas() {

    ifstream archivo("notas.lfp");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir notas.lfp" << endl;
        return;
    }

    listaNotas.clear();

    string linea;

    while (getline(archivo, linea)) {

        limpiarLinea(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        string carnet, codigo, nota, ciclo, anio;

        getline(ss, carnet, ',');
        getline(ss, codigo, ',');
        getline(ss, nota, ',');
        getline(ss, ciclo, ',');
        getline(ss, anio, ',');

        Nota n;
        n.carnet = stoi(carnet);
        n.codigoCurso = stoi(codigo);
        n.nota = stod(nota);
        n.ciclo = ciclo;
        n.anio = stoi(anio);

        listaNotas.push_back(n);
    }

    archivo.close();

    cout << "Notas cargadas: " << listaNotas.size() << endl;
}


// ================== REPORTES ==================

void generarReporteEstadisticasCurso() {

    ofstream html("reporte_estadisticas_curso.html");

    html << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html << "<title>Estadisticas por Curso</title>";
    html << "<style>";
    html << "body{font-family:Arial;background:#f4f6f9;margin:40px;}";
    html << "h1{background:#2c3e50;color:white;padding:20px;text-align:center;border-radius:8px;}";
    html << "table{width:100%;border-collapse:collapse;background:white;box-shadow:0 4px 8px rgba(0,0,0,0.1);}";
    html << "table,th,td{border:1px solid #bdc3c7;}";
    html << "th{background:#34495e;color:white;padding:12px;}";
    html << "td{padding:10px;text-align:center;}";
    html << "tr:nth-child(even){background:#ecf0f1;}";
    html << "</style></head><body>";

    html << "<h1>Reporte: Estadisticas Generales por Curso</h1>";

    time_t now = time(0);
    html << "<p style='text-align:right;font-size:12px;'>" << ctime(&now) << "</p>";

    html << "<table><tr>";
    html << "<th>Codigo</th><th>Nombre</th><th>Cantidad</th>";
    html << "<th>Promedio</th><th>Maxima</th><th>Minima</th>";
    html << "<th>Desviacion</th><th>Mediana</th></tr>";

    for (const Curso& curso : listaCursos) {

        vector<double> notasCurso;

        for (const Nota& nota : listaNotas) {
            if (nota.codigoCurso == curso.codigo)
                notasCurso.push_back(nota.nota);
        }

        if (notasCurso.empty()) continue;

        sort(notasCurso.begin(), notasCurso.end());

        int n = notasCurso.size();
        double suma = 0;
        for (double n : notasCurso) suma += n;
        double promedio = suma / n;
        double maximo = notasCurso.back();
        double minimo = notasCurso.front();

        double sumaVar = 0;
        for (double n : notasCurso)
            sumaVar += pow(n - promedio, 2);
        double desviacion = sqrt(sumaVar / n);

        double mediana;
        if (n % 2 == 0)
            mediana = (notasCurso[n/2 - 1] + notasCurso[n/2]) / 2;
        else
            mediana = notasCurso[n/2];

        html << "<tr>";
        html << "<td>" << curso.codigo << "</td>";
        html << "<td>" << curso.nombre << "</td>";
        html << "<td>" << n << "</td>";
        html << "<td>" << fixed << setprecision(2) << promedio << "</td>";
        html << "<td>" << maximo << "</td>";
        html << "<td>" << minimo << "</td>";
        html << "<td>" << desviacion << "</td>";
        html << "<td>" << mediana << "</td>";
        html << "</tr>";
    }

    html << "</table>";
    html << "<footer style='margin-top:30px;text-align:center;color:gray;'>Generado automaticamente por el Sistema de Analisis Academico</footer>";
    html << "</body></html>";

    html.close();
    cout << "Reporte generado correctamente.\n";
}

void generarReporteRendimientoEstudiante() {

    ofstream html("reporte_rendimiento_estudiante.html");

    html << "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html << "<title>Rendimiento por Estudiante</title>";
    html << "<style>";
    html << "body{font-family:Arial;background:#f4f6f9;margin:40px;}";
    html << "h1{background:#1e8449;color:white;padding:20px;text-align:center;border-radius:8px;}";
    html << "table{width:100%;border-collapse:collapse;background:white;}";
    html << "table,th,td{border:1px solid #bdc3c7;}";
    html << "th{background:#239b56;color:white;padding:12px;}";
    html << "td{padding:10px;text-align:center;}";
    html << "</style></head><body>";

    html << "<h1>Reporte: Rendimiento por Estudiante</h1>";

    html << "<table><tr>";
    html << "<th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th>";
    html << "<th>Promedio</th><th>Aprobados</th><th>Reprobados</th><th>Creditos</th></tr>";

    for (const Estudiante& est : listaEstudiantes) {

        double suma = 0;
        int total = 0, aprobados = 0, reprobados = 0, creditos = 0;

        for (const Nota& nota : listaNotas) {
            if (nota.carnet == est.carnet) {
                suma += nota.nota;
                total++;
                if (nota.nota >= 61) {
                    aprobados++;
                    for (const Curso& curso : listaCursos)
                        if (curso.codigo == nota.codigoCurso)
                            creditos += curso.creditos;
                } else reprobados++;
            }
        }

        if (total == 0) continue;

        double promedio = suma / total;

        html << "<tr>";
        html << "<td>" << est.carnet << "</td>";
        html << "<td>" << est.nombre << "</td>";
        html << "<td>" << est.carrera << "</td>";
        html << "<td>" << est.semestre << "</td>";
        html << "<td>" << fixed << setprecision(2) << promedio << "</td>";
        html << "<td>" << aprobados << "</td>";
        html << "<td>" << reprobados << "</td>";
        html << "<td>" << creditos << "</td>";
        html << "</tr>";
    }

    html << "</table></body></html>";
    html.close();
    cout << "Reporte generado correctamente.\n";
}

void generarReporteTop10() {

    ofstream html("reporte_top10.html");

    vector<pair<Estudiante,double>> promedios;

    for (const Estudiante& est : listaEstudiantes) {

        double suma = 0; int total = 0;

        for (const Nota& nota : listaNotas)
            if (nota.carnet == est.carnet) {
                suma += nota.nota;
                total++;
            }

        if (total > 0)
            promedios.push_back({est, suma/total});
    }

    sort(promedios.begin(), promedios.end(),
         [](auto& a, auto& b){ return a.second > b.second; });

    html << "<html><body><h1>Top 10 Mejores Estudiantes</h1>";
    html << "<table border='1'><tr><th>Pos</th><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th></tr>";

    for (int i = 0; i < 10 && i < promedios.size(); i++) {
        html << "<tr>";
        html << "<td>" << i+1 << "</td>";
        html << "<td>" << promedios[i].first.carnet << "</td>";
        html << "<td>" << promedios[i].first.nombre << "</td>";
        html << "<td>" << promedios[i].first.carrera << "</td>";
        html << "<td>" << promedios[i].first.semestre << "</td>";
        html << "<td>" << fixed << setprecision(2) << promedios[i].second << "</td>";
        html << "</tr>";
    }

    html << "</table></body></html>";
    html.close();
}

void generarReporteReprobacion() {

    ofstream html("reporte_reprobacion.html");

    html << "<html><body><h1>Cursos con Mayor Indice de Reprobacion</h1>";
    html << "<table border='1'><tr><th>Codigo</th><th>Nombre</th><th>Total</th><th>Aprobados</th><th>Reprobados</th><th>% Reprobacion</th></tr>";

    for (const Curso& curso : listaCursos) {

        int total = 0, reprobados = 0;

        for (const Nota& nota : listaNotas)
            if (nota.codigoCurso == curso.codigo) {
                total++;
                if (nota.nota < 61) reprobados++;
            }

        if (total == 0) continue;

        int aprobados = total - reprobados;
        double porcentaje = (reprobados * 100.0) / total;

        html << "<tr>";
        html << "<td>" << curso.codigo << "</td>";
        html << "<td>" << curso.nombre << "</td>";
        html << "<td>" << total << "</td>";
        html << "<td>" << aprobados << "</td>";
        html << "<td>" << reprobados << "</td>";
        html << "<td>" << fixed << setprecision(2) << porcentaje << "%</td>";
        html << "</tr>";
    }

    html << "</table></body></html>";
    html.close();
}

void generarReporteCarrera() {

    ofstream html("reporte_carrera.html");

    html << "<html><body><h1>Analisis por Carrera</h1>";
    html << "<table border='1'><tr><th>Carrera</th><th>Total Estudiantes</th><th>Promedio General</th></tr>";

    for (const Estudiante& est : listaEstudiantes) {

        double suma = 0; int total = 0;

        for (const Nota& nota : listaNotas)
            if (nota.carnet == est.carnet) {
                suma += nota.nota;
                total++;
            }

        if (total == 0) continue;

        html << "<tr>";
        html << "<td>" << est.carrera << "</td>";
        html << "<td>1</td>";
        html << "<td>" << fixed << setprecision(2) << suma/total << "</td>";
        html << "</tr>";
    }

    html << "</table></body></html>";
    html.close();
}


// ================== MAIN ==================
void mostrarMenu();

int main() {

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        switch(opcion) {

            case 1:
                cargarEstudiantes();
                break;

            case 2:
                cargarCursos();
                break;

            case 3:
                cargarNotas();
                break;

            case 4:
                generarReporteEstadisticasCurso();
                break;

            case 5:
                generarReporteRendimientoEstudiante();
                break;

            case 6:
                generarReporteTop10();
                break;

            case 7:
                generarReporteReprobacion();
                break;

            case 8:
                generarReporteCarrera();
                break;

            case 9:
                cout << "Saliendo del sistema..." << endl;
                break;

            default:
                cout << "Opcion invalida." << endl;
        }

        cout << endl;

    } while(opcion != 9);

    return 0;
}

void mostrarMenu() {
    cout << "==========================================\n";
    cout << "      SISTEMA DE ANALISIS ACADEMICO\n";
    cout << "==========================================\n";
    cout << "1. Cargar archivo de estudiantes\n";
    cout << "2. Cargar archivo de cursos\n";
    cout << "3. Cargar archivo de notas\n";
    cout << "4. Generar Reporte: Estadisticas por Curso\n";
    cout << "5. Generar Reporte: Rendimiento por Estudiante\n";
    cout << "6. Generar Reporte: Top 10 Mejores Estudiantes\n";
    cout << "7. Generar Reporte: Cursos con Mayor Reprobacion\n";
    cout << "8. Generar Reporte: Analisis por Carrera\n";
    cout << "9. Salir\n";
    cout << "Seleccione una opcion: ";
}