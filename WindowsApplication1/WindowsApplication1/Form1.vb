Public Class Form1

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Try
            SerialPort1.Open()
            Label1.Text = "Connected"
        Catch ex As Exception
            Label1.Text = "Not connected"
        End Try
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If SerialPort1.IsOpen Then
            SerialPort1.WriteLine("ON")
        Else
            MsgBox("Serial port not connected")
        End If
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        If SerialPort1.IsOpen Then
            SerialPort1.WriteLine("OFF")
        Else
            MsgBox("Serial port not connected")
        End If
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        Try
            SerialPort1.Close()
            Label1.Text = "Not connected"
        Catch ex As Exception
            Label1.Text = "connected"
        End Try
    End Sub
End Class
