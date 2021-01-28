package org.neecist


import android.app.Activity
import android.content.Context
import android.os.Bundle
import android.view.inputmethod.InputMethodManager
import android.widget.ImageButton
import androidx.appcompat.app.AppCompatActivity
import com.android.volley.AuthFailureError
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.textfield.TextInputEditText
import java.util.*


class MainActivity : AppCompatActivity() {
    var active = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        active = true;

        val leftArrow = findViewById<ImageButton>(R.id.leftArrow)
        val topArrow = findViewById<ImageButton>(R.id.topArrow)
        val rightArrow = findViewById<ImageButton>(R.id.rightArrow)
        val bottomArrow = findViewById<ImageButton>(R.id.bottomArrow)
        val extraData  = findViewById<ImageButton>(R.id.extraData)
        val sendButton = findViewById<ImageButton>(R.id.send)

        val inputText  =findViewById<TextInputEditText>(R.id.inputedText)





        // Instantiate the RequestQueue.
        val url = "http://192.168.1.1/"
        var string = ""
        val queue: RequestQueue = Volley.newRequestQueue(this)
        // Request a string response from the provided URL.

        val popupExtra = MaterialAlertDialogBuilder(this, R.style.Theme_Popup)
                .setTitle("Dados Extra")
        val popupMsg = MaterialAlertDialogBuilder(this, R.style.Theme_Popup)
                .setTitle("Mensagem alterada")

        var extraResponse : String
        val extraDataRequest = StringRequest(Request.Method.POST, url + "extraData",
                { response ->
                    val values = response.split(",")
                    extraResponse = "Valores lidos:\n Sensor da Direita: " + values[0] + ";\nSensor da Esquerda: " + values[1] + ";\n Sensor de Cima: " + values[2] + ";\nSensor de Baixo: " + values[3] + ";"
                    popupExtra.setMessage(extraResponse)
                    println("Good Response! <Extra Data>")
                    popupExtra.show()
                },
                {
                    popupExtra.setMessage("ERROR")
                    println("Bad Response! <Extra Data>")
                    popupExtra.show()
                })

        popupExtra
                .setNegativeButton("Fechar") { _, _ ->
                }
                .setPositiveButton("Refresh"){ _, _ ->
                    queue.add(extraDataRequest)
                }

        sendButton.setOnClickListener{
            val text = inputText.text

            val stringRequest = object : StringRequest(Method.POST, url + "print", { response ->
                println(response)
                println("Good Response! <Sent Button>")
                popupMsg.show()
            }, {
                println("Bad Response! <Sent Button>")
            }) {
                override fun getParams(): Map<String, String> {
                    val params: MutableMap<String, String> = HashMap()
                    params["fname"] = text.toString()
                    return params
                }

                @Throws(AuthFailureError::class)
                override fun getHeaders(): Map<String, String> {
                    val params: MutableMap<String, String> = HashMap()
                    params["Content-Type"] = "application/x-www-form-urlencoded"
                    return params
                }
            }
            println(stringRequest)
            inputText.setText("")
            popupMsg.setMessage("Nova messagem: ${text.toString()}")
            dismissKeyboard(this)
            queue.add(stringRequest)

        }


        leftArrow.setOnClickListener {
            val stringRequest = StringRequest(Request.Method.GET, url + "moveleft",
                    { response ->
                        println("Good Response! <Left Arrow>")
                    },
                    {
                        println("Bad Response! <Left Arrow>")
                    })
            queue.add(stringRequest)
        }

        topArrow.setOnClickListener {
            val stringRequest = StringRequest(Request.Method.GET, url + "moveup",
                    { response ->
                        println("Good Response! <Top Arrow>")
                    },
                    {
                        println("Bad Response! <Top Arrow>")
                    })
            queue.add(stringRequest)
        }

        rightArrow.setOnClickListener {
            val stringRequest = StringRequest(Request.Method.GET, url + "moveright",
                    { response ->
                        println("Good Response! <Right Arrow>")
                    },
                    {
                        println("Bad Response! <Right Arrow>")
                    })
            queue.add(stringRequest)
        }

        bottomArrow.setOnClickListener {
            val stringRequest = StringRequest(Request.Method.GET, url + "movedown",
                    { response ->
                        println("Good Response! <Bottom Arrow>")
                    },
                    {
                        println("Bad Response! <Bottom Arrow>")
                    })
            queue.add(stringRequest)
        }

        extraData.setOnClickListener{
            queue.add(extraDataRequest)
        }
    }
    override fun onStop() {
        super.onStop()
        active = false
    }
    fun dismissKeyboard(activity: Activity) {
        val imm = activity.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        if (null != activity.currentFocus) imm.hideSoftInputFromWindow(
                activity.currentFocus!!.applicationWindowToken, 0
        )
    }
}