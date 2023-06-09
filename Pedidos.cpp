#include "Pedidos.h"
#define _POSIX_SOURCE
/*
void Pedidos::aniadir_producto(string nombre_producto_entrada, int cantidad_entrada){
    Productos = Producto(nombre_producto_entrada,cantidad_entrada);
    contador_productos++;
}*/
void goto_archivos_erroneos(std::string nombre_archivo){
    const char* oldpath = ("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo).c_str();
    const char* newpath = ("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos_Erroneos/"+nombre_archivo).c_str();

    if (rename(oldpath, newpath) != 0) {
        perror("Error moving file");
    }
}

std::string Pedidos::to_string(){
    return std::to_string(numero_pedido) + "\t" +"("+std::to_string(Cliente->Prioridad)+")"+Cliente->Nombre;
}

std::string read_all(std::ifstream *file){
    std::string str = "";
    std::string linea;
    while(getline(*file,linea)){
        str += linea+"\n";
    }
    return str;
}

void write_error(std::ofstream* file,std::string str_arch,std::string error){
    *file<<error.c_str()<<"\n"<<str_arch.c_str();
    file->flush();
}


Pedidos *leer_archivo_pedido(std::string nombre_archivo,ListaClientes* Clientes,Almacen *almacen){
    int counter;
    std::string str = "";
    int numero_d_pedido;
    Pedidos *pedido_leido;
    std::ifstream archivoPedidos("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo);
    if (!archivoPedidos.is_open()) {
        std::cerr << "Failed to open Pedido file!" << std::endl;
        return NULL;
    } else {
        try {
            getline(archivoPedidos,str);//Se saca la primera linea (numero de pedido)
            numero_d_pedido = stoi(str);//Se convierte a int y se almacena en su variable correspondiente
        } catch (...) {
            std::cout<<"NUMERO DE PEDIDO INVALIDO"<<std::endl;
            archivoPedidos.seekg(0);
            std::string str_arc = read_all(&archivoPedidos);
            archivoPedidos.close();
            std::ofstream filee("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo);
            write_error(&filee,str_arc,"El pedido es invalido(No es un numero)(Linea 1)");
            filee.close();
            goto_archivos_erroneos(nombre_archivo);//Se manda el archivo a la dir con todos los archivos con errores
            return NULL;// El numero de pedido no es un numero retorna null
        }
        getline(archivoPedidos,str);//Se saca la segunda linea del archivo(Codigo del cliente)
        Cliente *Cliente_entrada = Clientes->buscar_cliente(str);
        if (Cliente_entrada == NULL){// Se confirma que el cliente realizando el pedido dentro de la lista de clientes
            std::cout<<"USUARIO INEXISTENTE"<<std::endl;
            archivoPedidos.seekg(0);
            std::string str_arc = read_all(&archivoPedidos);
            archivoPedidos.close();
            std::ofstream filee("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo);
            write_error(&filee,str_arc,"El usuario de este pedido no existe(Linea 2)");
            filee.close();
            goto_archivos_erroneos(nombre_archivo);//Se manda el archivo a la dir con todos los archivos con errores
            return NULL;
        } else {
            int i = 3;
            // Las siguientes lineas de codigo se ejecutan cuado el archivo tiene
            // *Numero de pedido valido
            // *Codigo de clientes valido
            // Por lo tanto se leeran los productos para el pedido
            std::string* linea;
            pedido_leido = new Pedidos(numero_d_pedido,Cliente_entrada);
            //El siguiente while es para leer los productos que van con el pedido
            while(getline(archivoPedidos,str)){
               // cout<<str<<endl;
                linea = adv_tokenizer(str,9);
                if (linea == NULL) //If en caso que en el archivo haya una linea con formato incorrecto
                    continue;
                try {//Se confirma que lo que este despues del tab sea un numero
                    counter = stoi(linea[1]);
                    if (counter<=0)
                        throw("A");
                } catch (...) {
                    std::cout<<"Cantidad de producto invalida o menor a 0"<<std::endl;
                    archivoPedidos.seekg(0);
                    std::string str_arc = read_all(&archivoPedidos);
                    archivoPedidos.close();
                    std::ofstream filee("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo);
                    write_error(&filee,str_arc,"Cantidad de producto invalida o menor o igual a 0(Linea "+std::to_string(i)+")");
                    filee.close();
                    goto_archivos_erroneos(nombre_archivo);//Se manda el archivo a la dir con todos los archivos con errores
                    return NULL;
                }
                Producto *p = almacen->existeProducto(linea[0]);
               // std::cout<<linea[0]<<std::endl;
                if(p != NULL){
                    i++;
                    pedido_leido->Productos->insertar(new Producto(linea[0],counter,p->ubicacion));
                } else {
                    std::cout<<"PRODUCTO INEXISTENTE"<<std::endl;
                    archivoPedidos.seekg(0);
                    std::string str_arc = read_all(&archivoPedidos);
                    archivoPedidos.close();
                    std::ofstream filee("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo);
                    write_error(&filee,str_arc,"El producto no existe(Linea "+std::to_string(i)+")");
                    filee.close();
                    goto_archivos_erroneos(nombre_archivo);//Se manda el archivo a la dir con todos los archivos con errores
                    return NULL;
                }

                //Se aniade el producto al pedido
            }
            archivoPedidos.close();
            //std::cout<<nombre_archivo<<std::endl;
            const char* oldpath = ("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos/"+nombre_archivo).c_str();
            const char* newpath = ("C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Pedidos_Leidos/"+nombre_archivo).c_str();

            if (rename(oldpath, newpath) != 0) {
                perror("Error moving file");
            }
            //std::cout<<pedido_leido->to_string()<<std::endl;
            return pedido_leido;
        }
    }
}

std::string Pedidos::getTime(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);
    std::string timeHour = std::to_string(ltm->tm_hour);
    std::string timeMinute = std::to_string(ltm->tm_min);
    std::string timeSecond = std::to_string(ltm->tm_sec);
    std::string hora = timeHour + ":" + timeMinute + ":" + timeSecond;
    return year + "-" + month + "-" + day + " " + hora;
}

void Pedidos::txtFactura(){
    std::ofstream myFile(factura);
    myFile << textoFactura;
    myFile.close();
}
void Pedidos::crearFactura(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);
    std::string timeHour = std::to_string(ltm->tm_hour);
    std::string timeMinute = std::to_string(ltm->tm_min);
    std::string timeSecond = std::to_string(ltm->tm_sec);
    std::string hora = timeHour + "-" + timeMinute + "-" + timeSecond;
    factura = "C:/Users/hdani/OneDrive/Escritorio/Tec semestre 1/datos/proyecto1/mio/Facturas/" +  std::to_string(numero_pedido) + "_" + Cliente->Codigo + "_"+ year + "-" + month + "-" + day + " " + hora + ".txt";
}

