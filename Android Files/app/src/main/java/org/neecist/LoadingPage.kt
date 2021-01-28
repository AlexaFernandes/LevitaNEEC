package org.neecist





import android.content.Intent
import android.os.Bundle
import android.view.animation.AnimationUtils
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.github.penfeizhou.animation.apng.APNGDrawable
import com.github.penfeizhou.animation.loader.ResourceStreamLoader
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import java.util.*
import kotlin.concurrent.schedule


class LoadingPage : AppCompatActivity() {
    var active = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        active = true;
        setContentView(R.layout.loading_page)
        val imageLevitaNEEC = findViewById<ImageView>(R.id.levitaneec)
        val loadingCenter = findViewById<ImageView>(R.id.animatedImg)
        val imageCircle1 = findViewById<ImageView>(R.id.circle1)
        val imageCircle2 = findViewById<ImageView>(R.id.circle2)
        val imageCircle3 = findViewById<ImageView>(R.id.circle3)


        val circle1Fade = AnimationUtils.loadAnimation(this, R.anim.circle1_fade)
        val circle2Fade = AnimationUtils.loadAnimation(this, R.anim.circle2_fade)
        val circle3Fade = AnimationUtils.loadAnimation(this, R.anim.circle3_fade)

        val loadedResource = ResourceStreamLoader(this, R.drawable.animated)
        val loadedAPNG = APNGDrawable(loadedResource)
        loadingCenter.setImageDrawable(loadedAPNG)

        imageCircle1.startAnimation(circle1Fade)
        imageCircle2.startAnimation(circle2Fade)
        imageCircle3.startAnimation(circle3Fade)



        // Instantiate the RequestQueue.
        val url = "http://192.168.1.1/online"
        var string = ""
        // create a daemon thread
        val timer = Timer("schedule", true);

        val mainIntent = Intent(this, MainActivity::class.java)

        imageLevitaNEEC.setOnClickListener {
            startActivity(mainIntent)
            finish()
        }
        val queue: RequestQueue = Volley.newRequestQueue(this)

        val popupMenu = MaterialAlertDialogBuilder(this, R.style.Theme_Popup)
                .setCancelable(false)
                .setTitle(getString(R.string.popupTitle))
                .setMessage(getString(R.string.popupDesc))


        // Request a string response from the provided URL.
        val stringRequest = StringRequest(Request.Method.GET, url,
                { response ->
                    // Display the first 500 characters of the response string.
                    string = response
                    if (response == "Online and Ready") {
                        startActivity(mainIntent)
                        finish()
                    }
                },
                {
                    if(active){
                        popupMenu.show()
                    }
                })

        popupMenu.setPositiveButton("Tentar outra vez") { _, _ ->
            timer.schedule(3000){
                queue.add(stringRequest)
                println("Retry")
            }
        }

        timer.schedule(3000){
            queue.add(stringRequest)
            println("SentRequest")
        }
    }
    override fun onStop() {
        super.onStop()
        active = false
    }
}

