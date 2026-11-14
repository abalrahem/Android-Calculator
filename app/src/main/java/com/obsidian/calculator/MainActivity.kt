package com.obsidian.calculator

import android.app.Activity
import android.graphics.Color
import android.graphics.Typeface
import android.graphics.drawable.GradientDrawable
import android.graphics.drawable.RippleDrawable
import android.content.res.ColorStateList
import android.os.Bundle
import android.widget.Button
import android.widget.GridLayout
import android.widget.TextView

class MainActivity : Activity() {

    companion object {
        init {
            System.loadLibrary("calculator")
        }
    }

    external fun pressButton(code: String)
    external fun getDisplay(): String
    external fun getExpr(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        window.statusBarColor = Color.parseColor("#0F0E17")
        window.navigationBarColor = Color.parseColor("#0F0E17")

        val tvExpr = findViewById<TextView>(R.id.tvExpr)
        val tvDisplay = findViewById<TextView>(R.id.tvDisplay)
        val grid = findViewById<GridLayout>(R.id.gridButtons)

        val buttons = listOf(
            "C" to "C",   "<-" to "BACK", "%" to "%",   "/" to "/",
            "7" to "7",   "8" to "8",     "9" to "9",   "x" to "x",
            "4" to "4",   "5" to "5",     "6" to "6",   "-" to "-",
            "1" to "1",   "2" to "2",     "3" to "3",   "+" to "+",
            "+/-" to "SIGN", "0" to "0",  "." to ".",   "=" to "="
        )

        for ((label, code) in buttons) {
            val btn = Button(this).apply {
                text = label
                textSize = 22f
                typeface = Typeface.DEFAULT_BOLD
                isAllCaps = false

                val (bgHex, txtHex, borderHex, rippleHex) = when (code) {
                    "=" -> listOf("#9333EA", "#FFFFFF", "#C084FC", "#EBC3FF")
                    "C" -> listOf("#3E1A2C", "#F472B6", "#783054", "#F472B6")
                    "/", "x", "-", "+", "%", "BACK" -> listOf("#2D1E44", "#C084FC", "#583884", "#C084FC")
                    else -> listOf("#1D1B2B", "#F8F7FF", "#302C48", "#8273C3")
                }

                setTextColor(Color.parseColor(txtHex))

                val shape = GradientDrawable().apply {
                    cornerRadius = 36f
                    setColor(Color.parseColor(bgHex))
                    setStroke(3, Color.parseColor(borderHex))
                }
                background = RippleDrawable(ColorStateList.valueOf(Color.parseColor(rippleHex)), shape, null)

                val params = GridLayout.LayoutParams().apply {
                    width = 0
                    height = 0
                    columnSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f)
                    rowSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f)
                    setMargins(12, 12, 12, 12)
                }
                layoutParams = params

                setOnClickListener {
                    pressButton(code)
                    tvDisplay.text = getDisplay()
                    tvExpr.text = getExpr()
                }
            }
            grid.addView(btn)
        }
    }
}
