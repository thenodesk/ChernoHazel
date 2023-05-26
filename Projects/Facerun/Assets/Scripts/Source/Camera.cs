using System;

using Hazel;

namespace Sandbox
{
    public class Camera : Entity
    {
        public Entity OtherEntity;

        public float DistanceFromPlayer = 5.0f;

        private float distanceFromPlayer = 0.0f;
        private Entity m_Player;

        void OnCreate()
        {
            m_Player = FindEntityByName("Player");
            distanceFromPlayer = Translation.Z;
        }

        void OnUpdate(float ts)
        {
            if (m_Player == null)
                return;

            Vector2 playerVelocity = m_Player.GetComponent<Rigidbody2DComponent>().LinearVelocity;
            float target = DistanceFromPlayer + playerVelocity.Length();
            distanceFromPlayer = Lerp(distanceFromPlayer, target, 4.0f * ts);

            Translation = new Vector3(m_Player.Translation.XY, distanceFromPlayer);


        }

        private float Lerp(float a, float b, float t)
        {
            return a * (1.0f - t) + (b * t);
        }
    }
}
