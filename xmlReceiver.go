package main

import (
	"bytes"
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

type MethodCall struct {
	MethodName string `xml:"methodName"`
}

func readMethodXML(data string) string {
	buf := bytes.NewBufferString(data)

	method := new(MethodCall)
	decoded := xml.NewDecoder(buf)

	err := decoded.Decode(method)
	if err != nil {
		fmt.Printf("Error: %v\n", err)
	}

	fmt.Printf("Title: %s\n", method.MethodName)
	return method.MethodName
}

func main() {
	fmt.Printf("Endpoint listening for http requests here:\n")
	fmt.Printf("http://localhost:8081/soapserver\n")
	http.HandleFunc("/consultarCPF", consultaCPF)
	http.HandleFunc("/submeter", postXML)
	log.Fatal(http.ListenAndServe(":8081", nil))
}

func postXML(w http.ResponseWriter, req *http.Request) {
	fmt.Printf("\n%s\n", req.Method)
	if req.Method != "POST" {
		w.Header().Set("Content-Type", "text/xml")
		w.Write([]byte("THIS METHOD DOESNT ACCEPT " + req.Method + " METHOD\n"))
		return
	}

	body, err := ioutil.ReadAll(req.Body)
	if err != nil {
		log.Fatal(err)
		return
	}

	xml := string(body[:])
	fmt.Printf("\n%s\n", xml)

	var method = readMethodXML(xml)
	if method != "submeter" {
		fmt.Printf("\nMétodo não encontrado\n")
	}
	fmt.Printf("\n%s\n", method)

	f, err := os.Create("arquivos/teste.xml")
	if err != nil {
		log.Fatal(err)
	}

	defer f.Close()

	fmt.Fprintf(f, xml)

}

func consultaCPF(w http.ResponseWriter, req *http.Request) {
	fmt.Printf("\n%s\n", req.Method)
	if req.Method != "GET" {
		w.Header().Set("Content-Type", "text/xml")
		w.Write([]byte("THIS METHOD DOESNT ACCEPT " + req.Method + " METHOD\n"))
		return
	}

	xml, err := ioutil.ReadAll(req.Body)
	if err != nil {
		log.Fatal(err)
		return
	}

	fmt.Printf("\n%s\n", xml)
}
