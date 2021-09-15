# ExemplosMPI

## Instalação do MPICH no Ubuntu

```
sudo apt install mpich
```

## Exemplo de compilação

``` 
mpicc mpi_hello.c -o mpihello
```

## Exemplo de execução

```
mpiexec -n 4 ./mpihello
```


