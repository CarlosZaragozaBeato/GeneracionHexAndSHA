package main

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"math/big"
	"os"
	"strconv"
	"strings"
	"sync"
)

type Resultado struct {
	decimalNumber int
	hexNumber     string
	sha256Value   string
	encontrado    bool
}
type Config struct {
	Inicio int `json:"inicio"`
}

func buscarNumeroConcurrente(inicio, fin int, wg *sync.WaitGroup, resultados chan<- *Resultado) {
	defer wg.Done()

	for i := inicio; i <= fin; i++ {
		paddedNumber := padLeft(strconv.Itoa(i))
		hexNumber, _ := decimalToHex(paddedNumber)
		result := strings.ToLower(hexNumber)
		value, _ := stringToSHA256(result)
		// fmt.Println("VALOR DECIMAL, ", paddedNumber)
		// fmt.Println("VALOR HEX ", hexNumber)
		// fmt.Println("VALOR SHA ", value)
		// fmt.Println()

		if value == NEXT_SEED {
			resultados <- &Resultado{
				decimalNumber: i,
				hexNumber:     hexNumber,
				sha256Value:   value,
				encontrado:    true,
			}
			return
		}
	}

	resultados <- &Resultado{encontrado: false}
}

func stringToSHA256(input string) (string, error) {
	hash := sha256.New()
	hash.Write([]byte(input))
	hashInBytes := hash.Sum(nil)
	hashInHex := hex.EncodeToString(hashInBytes)
	return hashInHex, nil

}

func padLeft(str string) string {
	count := 64 - len(str)
	if count <= 0 {
		return str
	}
	return strings.Repeat("0", count) + str
}

func decimalToHex(decimalStr string) (string, error) {
	decimal := new(big.Int)
	decimal.SetString(decimalStr, 10)
	return fmt.Sprintf("%X", decimal), nil
}

const NEXT_SEED = "2b8e9f51b7491aae5f189449be957b40b1f25ebc283c3b95352491cd260e3c78"
const PATH = "C:/Users/carlo/Desktop/GeneracionHexAndSHA/inicio.json"

const INCREMENT = 1000000000

func main() {

	for i := 0; i < 5; i++ {
		bytesJSON, _ := os.ReadFile(PATH)
		var config Config
		json.Unmarshal(bytesJSON, &config)
		number := config.Inicio

		var wg sync.WaitGroup

		resultados := make(chan *Resultado, 4)

		wg.Add(1)
		go buscarNumeroConcurrente(number, number+INCREMENT, &wg, resultados)
		wg.Add(1)
		go buscarNumeroConcurrente(number+INCREMENT, number+INCREMENT*2, &wg, resultados)
		wg.Add(1)
		go buscarNumeroConcurrente(number+INCREMENT*2, number+INCREMENT*3, &wg, resultados)
		wg.Add(1)
		go buscarNumeroConcurrente(number+INCREMENT*3, number+INCREMENT*4, &wg, resultados)

		go func() {
			wg.Wait()
			close(resultados)
		}()

		for resultado := range resultados {
			if resultado.encontrado {
				fmt.Println("ENCONTRADO")
				fmt.Println("NUMERO HEXADECIMAL: ", resultado.hexNumber)
				fmt.Println("NUMERO DECIMAL: ", resultado.decimalNumber)
				fmt.Println("NUMERO sha256: ", resultado.sha256Value)
			}
		}

		config.Inicio += INCREMENT * 4
		nuevoBytesJSON, _ := json.MarshalIndent(config, "", "  ")
		os.WriteFile(PATH, nuevoBytesJSON, 0644)
		fmt.Println("Todas las goroutines han terminado. index -> ", i)
	}
}
